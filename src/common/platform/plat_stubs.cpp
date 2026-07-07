// LH-P5.2: definitions of the severed-graphics host stubs. Compiled ONLY in the
// Linux host build (added to the LH-P6 CMake source list); never compiled on
// Windows. See plat_stubs.h for the rationale.
#ifndef _WIN32

#include "plat_stubs.h"
#include "plat_gfx_stubs.h"
#include "myfile.h" // host file I/O seam (open2/get/lof/close), case-resolving

#include <cstdint>
#include <cstdlib>
#include <cstring>

// UI panels are never used by the dedicated host; the panel globals are
// statically zero-initialized, so initialization is a no-op here.
void frame_init(void) {}

// The headless host has no display surface; report "success" so any gated
// caller proceeds.
bool setupddraw(void) { return true; }

// ---------------------------------------------------------------------------
// Headless custom-portrait BMP loader.
//
// The dedicated host has no DirectDraw/GDI, but it MUST still read the custom
// portrait bitmaps it wrote under .\save\port\N.BMP so it can serve them to
// clients: the "request portrait data" path in loop_host_part_b_admin.cpp
// compresses HOST_portrait_data[] (3584 RGB565 shorts per portrait) onto the
// wire, and the portrait loader in loop_host_part_b_charcreate.cpp populates
// that array from loadimage(...)->o. The old stub returned null, so the Linux
// host never loaded or served any custom portrait.
//
// This reimplements exactly what the Windows path produced. There,
// loadimage() did LoadImage()+BitBlt into newsurf(w,h,SURF_SYSMEM16) — a
// top-down, tightly (DWORD-)packed 16-bit RGB565 surface — and the portrait
// code memcpy's the first 7168 bytes of surf->o. We produce byte-identical
// pixels straight from the on-disk BMP, with no GDI dependency.
//
// Scope: the host only ever writes/reads its own portraits, which are always
// 24-bit uncompressed (BI_RGB) BMPs (BMPDATA.BMP: 56x64, biBitCount=24,
// bottom-up). We also accept 32-bit BI_RGB for robustness; anything else
// returns null (the correct headless degradation the old stub provided).
//
// Fidelity: the upload writer stored B=(b5<<3), G=(g6<<2), R=(r5<<3), so
// R>>3 / G>>2 / B>>3 here recover the EXACT original 5:6:5 channels a Windows
// host would have served — the compressed wire bytes are unchanged.
// ---------------------------------------------------------------------------

// The portrait consumer fixed-copies this many bytes (56*64*2) from surf->o.
// The output buffer is padded to at least this size so that copy never reads
// out of bounds even for an unexpectedly small BMP.
static const long kPortraitCopyBytes = 7168;

static inline uint32_t plat_rd32(const unsigned char *p) {
    return (uint32_t) p[0] | ((uint32_t) p[1] << 8) | ((uint32_t) p[2] << 16) | ((uint32_t) p[3] << 24);
}

static inline uint16_t plat_rd16(const unsigned char *p) {
    return (uint16_t) ((uint32_t) p[0] | ((uint32_t) p[1] << 8));
}

surf *loadimage(const char *name, int /*memflags*/) {
    if (name == nullptr) return nullptr;

    // Read the whole BMP through the case-resolving host file seam. OF_READ ->
    // O_RDONLY, so a read-only file on the mounted volume still opens.
    file *f = open2(name, OF_READ);
    if (f->h == HFILE_ERROR) {
        close(f);
        return nullptr;
    }
    long len = lof(f);
    if (len < 54) { // BITMAPFILEHEADER(14) + BITMAPINFOHEADER(40)
        close(f);
        return nullptr;
    }
    unsigned char *raw = (unsigned char *) malloc((size_t) len);
    if (raw == nullptr) {
        close(f);
        return nullptr;
    }
    get(f, raw, len);
    close(f);

    // Parse BITMAPFILEHEADER + BITMAPINFOHEADER (little-endian, byte-wise).
    if (raw[0] != 'B' || raw[1] != 'M') { free(raw); return nullptr; }
    const uint32_t offBits = plat_rd32(raw + 10);
    const uint32_t biSize  = plat_rd32(raw + 14);
    const int32_t  biW     = (int32_t) plat_rd32(raw + 18);
    const int32_t  biH     = (int32_t) plat_rd32(raw + 22);
    const uint16_t biBit   = plat_rd16(raw + 28);
    const uint32_t biComp  = plat_rd32(raw + 30);

    if (biSize < 40) { free(raw); return nullptr; }       // need BITMAPINFOHEADER+
    if (biComp != 0) { free(raw); return nullptr; }       // BI_RGB only
    if (biBit != 24 && biBit != 32) { free(raw); return nullptr; }

    const bool bottomUp = (biH > 0);
    const long width  = biW;
    const long height = (biH < 0) ? -(long) biH : (long) biH;
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) { free(raw); return nullptr; }

    const int  bytespp   = biBit / 8;
    const long srcStride = (width * bytespp + 3L) & ~3L;   // BMP rows are DWORD-aligned
    if ((long) offBits < 54 || (long) offBits + srcStride * height > len) { free(raw); return nullptr; }

    // Output: top-down, tightly packed 16-bit RGB565 (pitch == width*2, which
    // equals the Windows DWORD-aligned pitch for the 56-wide portraits).
    long outBytes = width * height * 2;
    if (outBytes < kPortraitCopyBytes) outBytes = kPortraitCopyBytes;

    // Reuse a single owned buffer + surf across calls: the consumer copies out
    // of surf->o immediately after each call, so no per-load allocation leaks.
    static surf hostsurf;
    static unsigned char *pixbuf = nullptr;
    static long pixcap = 0;
    if (outBytes > pixcap) {
        free(pixbuf);
        pixbuf = (unsigned char *) malloc((size_t) outBytes);
        if (pixbuf == nullptr) { pixcap = 0; free(raw); return nullptr; }
        pixcap = outBytes;
    }
    memset(pixbuf, 0, (size_t) outBytes);
    unsigned short *out = (unsigned short *) pixbuf;

    for (long r = 0; r < height; r++) {
        const long srcRow = bottomUp ? (height - 1 - r) : r;
        const unsigned char *sp = raw + offBits + srcRow * srcStride;
        unsigned short *dp = out + r * width;
        for (long c = 0; c < width; c++) {
            const unsigned char b  = sp[0];
            const unsigned char g  = sp[1];
            const unsigned char rr = sp[2];
            sp += bytespp;
            dp[c] = (unsigned short) (((rr >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
        }
    }

    free(raw);
    hostsurf.o = pixbuf;
    return &hostsurf;
}

#endif // !_WIN32

