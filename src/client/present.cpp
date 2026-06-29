// MPRES-P1 — Modern swap-chain presenter implementation (Direct3D 11 + DXGI).
//
// Implements the u6o::client present facade declared in present.h. A single
// file-static RAII `Presenter` (ComPtr members) owns the device, swap chain,
// dynamic RGB565 texture, sampler and shaders, and is driven entirely from
// `present(const surf*)` — there is no WndProc plumbing: the swap chain adapts to
// the live client size every frame via ResizeBuffers.
//
// Behavior preservation:
//   * The source `surf::o` RGB565 bytes are NEVER modified.
//   * The letterbox dst rect + scale are computed with the EXACT blit_letterbox
//     formula and published to blit_offx/blit_offy/blit_scale (+ clientW/clientH),
//     so mouse mapping is byte-identical to the legacy path.
//   * Point sampling reproduces the legacy COLORONCOLOR StretchBlt; a 1:1 present
//     (s==1.0) is a straight copy like the legacy BitBlt branch.
//   * No wire/.sav/RNG impact; client render only. U6O_VERSION unchanged.
//
// Default OFF: only reached when g_present_modern != 0 (set by the "modernpresent"
// command-line switch). On ANY failure this returns false and the caller uses the
// legacy DirectDraw present, so enabling the flag can never break rendering.
//
// See docs/modernization/MPRES-P1-presenter.md (letterbox math + API decision)
// and docs/plans/in-progress/plan-modernPresenter.md (phase MPRES-P1).

#ifdef CLIENT

// D3D11 + the DXGI 1.2 flip-model swap chain (IDXGIFactory2::CreateSwapChainForHwnd,
// DXGI_SWAP_EFFECT_FLIP_DISCARD) need Windows 8/10 SDK symbols. The repo's
// stdafx.h targets WINVER 0x0501 (XP); this TU does not include stdafx.h, so we
// raise the target here, before any Windows header is pulled in.
#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <cstdint>
#include <cstring>

// Pull the on-disk `surf` layout (DDSURFACEDESC2 d; ... union { o/o1/o2 }; ...).
#include "myddraw.h"
#include "present.h"

// Link the modern graphics libs from the TU itself so no CMake link changes are
// needed (mirrors the project's local #pragma comment(lib) style).
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Globals owned elsewhere (myddraw.cpp / globals.inc). The presenter consumes the
// same hWnd and publishes the same transform globals as blit_letterbox.
extern HWND hWnd;
extern long blit_offx;
extern long blit_offy;
extern double blit_scale;
extern long clientW;
extern long clientH;

namespace {

using Microsoft::WRL::ComPtr;

// Fullscreen-triangle vertex shader from SV_VertexID (3 verts, no VB/IB). Emits
// clip-space position covering the whole render target and UV in [0,1].
static const char* kVsSrc =
    "struct VSOut { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; };\n"
    "VSOut main(uint vid : SV_VertexID) {\n"
    "    VSOut o;\n"
    "    float2 uv = float2((vid << 1) & 2, vid & 2);\n"
    "    o.uv  = uv;\n"
    "    o.pos = float4(uv * float2(2.0, -2.0) + float2(-1.0, 1.0), 0.0, 1.0);\n"
    "    return o;\n"
    "}\n";

// Point-sample the RGB565 texture. The B5G6R5_UNORM SRV expands to rgb with a=1;
// the BGRA8 render target stores the display-format pixel.
static const char* kPsSrc =
    "Texture2D    tex : register(t0);\n"
    "SamplerState smp : register(s0);\n"
    "float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET {\n"
    "    return tex.Sample(smp, uv);\n"
    "}\n";

/// @brief RAII owner of the D3D11/DXGI present resources. One file-static instance.
class Presenter {
public:
    /// Present `s` (RGB565) letterboxed; returns false to request legacy fallback.
    bool present(const surf* s) {
        if (s == nullptr) return false;

        // (2) Live client size. Publish raw values exactly like blit_letterbox,
        //     clamp only for the buffer/scale math.
        RECT cr;
        if (!GetClientRect(hWnd, &cr)) return false;
        const long cWraw = cr.right - cr.left;
        const long cHraw = cr.bottom - cr.top;
        clientW = cWraw;
        clientH = cHraw;
        const UINT bufW = (cWraw < 1) ? 1u : static_cast<UINT>(cWraw);
        const UINT bufH = (cHraw < 1) ? 1u : static_cast<UINT>(cHraw);

        // (1) Lazy device/swap-chain init.
        if (!ready_) {
            if (!initDevice(bufW, bufH)) {
                shutdown();   // leave nothing half-built
                return false; // caller uses legacy present
            }
        }

        // (2 cont.) Adapt the swap chain to the current client size.
        if (bufW != swapW_ || bufH != swapH_) {
            if (!resize(bufW, bufH)) return false;
        }

        // (3) Letterbox math — MUST match blit_letterbox exactly.
        const long srcW = static_cast<long>(s->d.dwWidth);
        const long srcH = static_cast<long>(s->d.dwHeight);
        long cW = cWraw < 1 ? 1 : cWraw;
        long cH = cHraw < 1 ? 1 : cHraw;
        double sx = static_cast<double>(cW) / static_cast<double>(srcW);
        double sy = static_cast<double>(cH) / static_cast<double>(srcH);
        double scale = (sx < sy) ? sx : sy;
        if (scale > 1.0) scale = 1.0; // never upscale past native
        if (scale <= 0.0) scale = 1.0;
        const long dstW = static_cast<long>(srcW * scale + 0.5);
        const long dstH = static_cast<long>(srcH * scale + 0.5);
        const long dstX = (cW - dstW) / 2;
        const long dstY = (cH - dstH) / 2;
        blit_offx = dstX;
        blit_offy = dstY;
        blit_scale = scale;

        // (4) (Re)create the dynamic RGB565 texture at srcW x srcH.
        if (!tex_ || texW_ != static_cast<UINT>(srcW) ||
            texH_ != static_cast<UINT>(srcH)) {
            if (!createTexture(static_cast<UINT>(srcW),
                               static_cast<UINT>(srcH)))
                return false;
        }

        // (5) Upload rows honoring lPitch on the source and RowPitch on the dest.
        D3D11_MAPPED_SUBRESOURCE mapped;
        if (FAILED(ctx_->Map(tex_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)))
            return false;
        {
            const uint8_t* srcRow = reinterpret_cast<const uint8_t*>(s->o);
            uint8_t* dstRow = static_cast<uint8_t*>(mapped.pData);
            const size_t rowBytes = static_cast<size_t>(srcW) * 2u; // RGB565
            const long srcPitch = static_cast<long>(s->d.lPitch);
            for (long y = 0; y < srcH; ++y) {
                memcpy(dstRow, srcRow, rowBytes);
                srcRow += srcPitch;
                dstRow += mapped.RowPitch;
            }
        }
        ctx_->Unmap(tex_.Get(), 0);

        // (6) Clear the whole RT to black — this paints the letterbox bars.
        const float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        ctx_->ClearRenderTargetView(rtv_.Get(), black);

        // (7) Viewport = dst rect; bind and draw the fullscreen triangle.
        D3D11_VIEWPORT vp;
        vp.TopLeftX = static_cast<float>(dstX);
        vp.TopLeftY = static_cast<float>(dstY);
        vp.Width = static_cast<float>(dstW);
        vp.Height = static_cast<float>(dstH);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        ctx_->RSSetViewports(1, &vp);
        ctx_->OMSetRenderTargets(1, rtv_.GetAddressOf(), nullptr);
        ctx_->IASetInputLayout(nullptr);
        ctx_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ctx_->VSSetShader(vs_.Get(), nullptr, 0);
        ctx_->PSSetShader(ps_.Get(), nullptr, 0);
        ctx_->PSSetShaderResources(0, 1, srv_.GetAddressOf());
        ctx_->PSSetSamplers(0, 1, samp_.GetAddressOf());
        ctx_->Draw(3, 0);

        // (8) Present with vsync.
        if (FAILED(swap_->Present(1, 0))) return false;
        return true;
    }

    /// Release everything; safe if never initialized.
    void shutdown() {
        if (ctx_) ctx_->ClearState();
        samp_.Reset();
        ps_.Reset();
        vs_.Reset();
        srv_.Reset();
        tex_.Reset();
        rtv_.Reset();
        swap_.Reset();
        ctx_.Reset();
        device_.Reset();
        ready_ = false;
        swapW_ = swapH_ = 0;
        texW_ = texH_ = 0;
    }

private:
    bool initDevice(UINT w, UINT h) {
        UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        const D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,  D3D_FEATURE_LEVEL_9_1,
        };
        D3D_FEATURE_LEVEL got = D3D_FEATURE_LEVEL_9_1;
        HRESULT hr = D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levels,
            ARRAYSIZE(levels), D3D11_SDK_VERSION, device_.GetAddressOf(), &got,
            ctx_.GetAddressOf());
        if (FAILED(hr)) return false;

        // Reach the DXGI factory that birthed this device's adapter.
        ComPtr<IDXGIDevice> dxgiDevice;
        if (FAILED(device_.As(&dxgiDevice))) return false;
        ComPtr<IDXGIAdapter> adapter;
        if (FAILED(dxgiDevice->GetAdapter(adapter.GetAddressOf()))) return false;
        ComPtr<IDXGIFactory2> factory;
        if (FAILED(adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf()))))
            return false;

        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = w;
        sd.Height = h;
        sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.Stereo = FALSE;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 2;
        sd.Scaling = DXGI_SCALING_STRETCH;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sd.Flags = 0;
        if (FAILED(factory->CreateSwapChainForHwnd(device_.Get(), hWnd, &sd,
                                                   nullptr, nullptr,
                                                   swap_.GetAddressOf())))
            return false;
        // Keep DXGI from hijacking Alt+Enter (the window owns its own resize).
        factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

        swapW_ = w;
        swapH_ = h;
        if (!createRTV()) return false;
        if (!createShaders()) return false;
        if (!createSampler()) return false;
        ready_ = true;
        return true;
    }

    bool createRTV() {
        ComPtr<ID3D11Texture2D> back;
        if (FAILED(swap_->GetBuffer(0, IID_PPV_ARGS(back.GetAddressOf()))))
            return false;
        rtv_.Reset();
        return SUCCEEDED(device_->CreateRenderTargetView(
            back.Get(), nullptr, rtv_.GetAddressOf()));
    }

    bool resize(UINT w, UINT h) {
        rtv_.Reset(); // must drop the RTV before ResizeBuffers
        if (FAILED(swap_->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0)))
            return false;
        if (!createRTV()) return false;
        swapW_ = w;
        swapH_ = h;
        return true;
    }

    bool createTexture(UINT w, UINT h) {
        D3D11_TEXTURE2D_DESC td = {};
        td.Width = w;
        td.Height = h;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_B5G6R5_UNORM; // native RGB565 — no CPU convert
        td.SampleDesc.Count = 1;
        td.Usage = D3D11_USAGE_DYNAMIC;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        tex_.Reset();
        srv_.Reset();
        if (FAILED(device_->CreateTexture2D(&td, nullptr, tex_.GetAddressOf())))
            return false;
        if (FAILED(device_->CreateShaderResourceView(tex_.Get(), nullptr,
                                                     srv_.GetAddressOf())))
            return false;
        texW_ = w;
        texH_ = h;
        return true;
    }

    bool createShaders() {
        ComPtr<ID3DBlob> vsb, psb, err;
        if (FAILED(D3DCompile(kVsSrc, strlen(kVsSrc), "u6o_present_vs", nullptr,
                              nullptr, "main", "vs_4_0", 0, 0,
                              vsb.GetAddressOf(), err.GetAddressOf())))
            return false;
        if (FAILED(device_->CreateVertexShader(vsb->GetBufferPointer(),
                                              vsb->GetBufferSize(), nullptr,
                                              vs_.GetAddressOf())))
            return false;
        err.Reset();
        if (FAILED(D3DCompile(kPsSrc, strlen(kPsSrc), "u6o_present_ps", nullptr,
                              nullptr, "main", "ps_4_0", 0, 0,
                              psb.GetAddressOf(), err.GetAddressOf())))
            return false;
        if (FAILED(device_->CreatePixelShader(psb->GetBufferPointer(),
                                             psb->GetBufferSize(), nullptr,
                                             ps_.GetAddressOf())))
            return false;
        return true;
    }

    bool createSampler() {
        D3D11_SAMPLER_DESC sdsc = {};
        sdsc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // == COLORONCOLOR StretchBlt
        sdsc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sdsc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sdsc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sdsc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sdsc.MaxLOD = D3D11_FLOAT32_MAX;
        return SUCCEEDED(
            device_->CreateSamplerState(&sdsc, samp_.GetAddressOf()));
    }

    ComPtr<ID3D11Device> device_;
    ComPtr<ID3D11DeviceContext> ctx_;
    ComPtr<IDXGISwapChain1> swap_;
    ComPtr<ID3D11RenderTargetView> rtv_;
    ComPtr<ID3D11Texture2D> tex_;
    ComPtr<ID3D11ShaderResourceView> srv_;
    ComPtr<ID3D11SamplerState> samp_;
    ComPtr<ID3D11VertexShader> vs_;
    ComPtr<ID3D11PixelShader> ps_;
    bool ready_ = false;
    UINT swapW_ = 0, swapH_ = 0;
    UINT texW_ = 0, texH_ = 0;
};

// Single file-static instance; ComPtr members release on process teardown if
// present_modern_shutdown() was not called explicitly.
Presenter g_presenter;

} // namespace

namespace u6o {
namespace client {

bool present_modern(const surf* s) { return g_presenter.present(s); }
void present_modern_shutdown() { g_presenter.shutdown(); }

} // namespace client
} // namespace u6o

#endif // CLIENT
