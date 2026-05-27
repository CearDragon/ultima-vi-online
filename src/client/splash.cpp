// splash.cpp — implementation of the pre-game splash window.
//
// Uses GDI+ to load any common image format (PNG, JPG, BMP, GIF) so the
// product team can drop a `splash.png` next to the EXE (or under
// assets/images/) without needing a custom asset pipeline.
//
// Design notes
// ------------
// * The splash uses its own WNDCLASS distinct from the game window so we
//   don't tangle with the main WndProc lifecycle (which is large and
//   stateful, see u6o7.cpp).
// * Window style is WS_POPUP + WS_EX_TOOLWINDOW so it has no chrome and
//   does not appear on the taskbar. The main game window owns the taskbar
//   entry once it's created later in setup_client.inc.
// * The window is sized to the image's native dimensions (clamped to a
//   reasonable max so an over-large asset does not fill the desktop) and
//   centered on the monitor that contains the cursor.
// * We always pump messages during the wait so the splash repaints if
//   covered, and so Windows does not flag the process as "Not Responding".
// * Every failure path falls through to a guaranteed Sleep(minDuration) so
//   the game-start contract holds even if GDI+/image load breaks.
#include "splash.h"
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
namespace u6o { namespace client { namespace splash {
namespace {
constexpr int kMaxSplashW = 1280;
constexpr int kMaxSplashH = 720;
constexpr int kFallbackW = 640;
constexpr int kFallbackH = 360;
const wchar_t kClassName[] = L"U6OSplashWindow";
struct SplashState {
    Gdiplus::Image* image = nullptr;
    int             width = kFallbackW;
    int             height = kFallbackH;
};
SplashState* g_state = nullptr;
LRESULT CALLBACK SplashWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps{};
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc{};
            GetClientRect(hwnd, &rc);
            HBRUSH black = (HBRUSH)GetStockObject(BLACK_BRUSH);
            FillRect(hdc, &rc, black);
            if (g_state && g_state->image &&
                g_state->image->GetLastStatus() == Gdiplus::Ok) {
                Gdiplus::Graphics g(hdc);
                g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
                g.DrawImage(g_state->image, 0, 0, rc.right, rc.bottom);
            } else {
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, RGB(220, 220, 220));
                const wchar_t kFallbackText[] = L"Ultima VI Online";
                DrawTextW(hdc, kFallbackText, -1, &rc,
                          DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_DESTROY:
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}
Gdiplus::Image* LoadFirstAvailable(const wchar_t* const* paths) {
    if (!paths) return nullptr;
    for (const wchar_t* const* p = paths; *p; ++p) {
        Gdiplus::Image* img = new Gdiplus::Image(*p);
        if (img && img->GetLastStatus() == Gdiplus::Ok &&
            img->GetWidth() > 0 && img->GetHeight() > 0) {
            return img;
        }
        delete img;
    }
    return nullptr;
}
void CenterRectOnCursorMonitor(int w, int h, int& outX, int& outY) {
    POINT cursor{};
    GetCursorPos(&cursor);
    HMONITOR hMon = MonitorFromPoint(cursor, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mi{ sizeof(MONITORINFO) };
    if (GetMonitorInfoW(hMon, &mi)) {
        outX = mi.rcWork.left + ((mi.rcWork.right - mi.rcWork.left) - w) / 2;
        outY = mi.rcWork.top + ((mi.rcWork.bottom - mi.rcWork.top) - h) / 2;
    } else {
        outX = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
        outY = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
    }
}
void PumpUntilDeadline(DWORD deadlineTickCount) {
    MSG msg{};
    for (;;) {
        DWORD now = GetTickCount();
        if (now >= deadlineTickCount) return;
        DWORD remaining = deadlineTickCount - now;
        DWORD wait = MsgWaitForMultipleObjects(0, nullptr, FALSE, remaining,
                                               QS_ALLINPUT);
        if (wait == WAIT_TIMEOUT) return;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
}
const wchar_t* const kDefaultSearchPaths[] = {
    L".\\splash.png",
    L".\\dr\\splash.png",
    L".\\assets\\images\\splash.png",
    L"assets\\images\\splash.png",
    nullptr,
};
}  // namespace
const wchar_t* const* DefaultSearchPaths() {
    return kDefaultSearchPaths;
}
void Run(HINSTANCE hInstance,
         const wchar_t* const* searchPaths,
         DWORD minDurationMs) {
    const DWORD startTick = GetTickCount();
    const DWORD deadlineTick = startTick + minDurationMs;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken = 0;
    Gdiplus::Status gdiStatus = Gdiplus::GdiplusStartup(
        &gdiplusToken, &gdiplusStartupInput, nullptr);
    const bool gdiOk = (gdiStatus == Gdiplus::Ok);
    SplashState state;
    if (gdiOk) {
        state.image = LoadFirstAvailable(searchPaths);
        if (state.image) {
            UINT imgW = state.image->GetWidth();
            UINT imgH = state.image->GetHeight();
            double scale = 1.0;
            if ((int)imgW > kMaxSplashW)
                scale = (double)kMaxSplashW / (double)imgW;
            if ((int)imgH * scale > kMaxSplashH)
                scale = (double)kMaxSplashH / (double)imgH;
            state.width = (int)(imgW * scale);
            state.height = (int)(imgH * scale);
            if (state.width <= 0)  state.width = kFallbackW;
            if (state.height <= 0) state.height = kFallbackH;
        }
    }
    g_state = &state;
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = SplashWndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = kClassName;
    RegisterClassExW(&wc);
    int posX = 0, posY = 0;
    CenterRectOnCursorMonitor(state.width, state.height, posX, posY);
    HWND hwnd = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        kClassName,
        L"Ultima VI Online",
        WS_POPUP,
        posX, posY, state.width, state.height,
        nullptr, nullptr, hInstance, nullptr);
    if (hwnd) {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        PumpUntilDeadline(deadlineTick);
        DestroyWindow(hwnd);
    } else {
        DWORD now = GetTickCount();
        if (now < deadlineTick) Sleep(deadlineTick - now);
    }
    MSG msg{};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    g_state = nullptr;
    delete state.image;
    state.image = nullptr;
    if (gdiOk) Gdiplus::GdiplusShutdown(gdiplusToken);
}
}}}  // namespace u6o::client::splash
