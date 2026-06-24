# Memory Leak Quick-Reference: Code Locations & Fixes

This document maps each identified leak to its location and the fix needed.

## Critical Leaks (Fix immediately in MM-P2, MM-P3, MM-P4)

### 1. DirectDraw Surface Malloc Never Freed (MM-P2)

**File:** `src/client/myddraw.cpp`

**Problem (lines 186–202):**
```cpp
// In surfstruct():
ts = (surf *) malloc(sizeof(surf));
// ... initialize ...
return ts;

// Later at line 202:
//free((void*)ts);  <-- COMMENTED OUT!
```

**Fix:** Restore the free() and ensure it's called on shutdown or when surfaces are purged.

---

### 2. purgesurfaces() Incomplete Cleanup (MM-P2)

**File:** `src/client/myddraw.cpp:761–769`

**Problem:**
```cpp
void purgesurfaces() {
    for (i=0; i<surfcount; i++) {
        if (surflist[i]->s) {
            surflist[i]->s->Release();  // <-- Releases COM object
            // But doesn't free the malloc'd surf struct!
        }
    }
}
```

**Fix:** Add `free((void*)surflist[i]);` after `Release()`.

---

### 3. Font Leak + `-l` Workaround (MM-P3)

**Files:** 
- `src/client/myddraw.cpp:691–717` (documented leak)
- `src/client/loop/loop_client_part_refresh_tail.cpp:380–406` (`-l` workaround loop)
- `src/common/u6o7.cpp:349–351` (flag parsing)
- `src/common/globals.inc:303` (leak variable)

**Problem:**
```cpp
// myddraw.cpp line 692:
void CreateAllFonts() {
    // MEMLEAKING A LOT! thats why I added delete object and it works, 
    // but the font is fucked up if deleted right away
    // (SelectObject/GetDC/ReleaseDC pattern issue)
}

// loop_client_part_refresh_tail.cpp line 381:
if (leak) {
    DeleteObject(fnt1);  // Delete
    DeleteObject(fnt2);
    // ... all 7 ...
    fnt1 = CreateFont(...);  // Recreate
    fnt2 = CreateFont(...);
    // ... all 7 ...
}
// ^ This loop runs EVERY FRAME (60 times/sec)!
```

**Fix (three-part):**
1. Fix the SelectObject/GetDC/ReleaseDC pattern so fonts can be deleted cleanly
2. Move font creation to setup (`setup_client.inc`) instead of every frame
3. Remove the `-l` flag parsing and workaround loop

---

### 4. Socket Buffer Structure Leaks (MM-P4)

**File:** `src/common/function_both.cpp:176–184, 225–264, 293–300`

**Problem:**
```cpp
// Socket RX allocation (line 176–184):
char *socketclient_ri_d = (char *)malloc(size);
socketclient_ri->d = socketclient_ri_d;
// Later, socketclient_ri is NEVER freed on disconnect!

// In sockets_disconnect() (line 225–264):
void sockets_disconnect(int connindex) {
    closesocket(socketclient[connindex].s);  // Close socket
    // But socketclient_si[connindex] and socketclient_ri[connindex] are never freed!
}
```

**Fix:** In `sockets_disconnect()`, add cleanup:
```cpp
if (socketclient_ri[connindex]) free(socketclient_ri[connindex]);
if (socketclient_si[connindex]) free(socketclient_si[connindex]);
socketclient_ri[connindex] = NULL;
socketclient_si[connindex] = NULL;
```

---

## Medium-Impact Leaks (Fix in MM-P5, MM-P6)

### 5. Sound Buffer Management (MM-P5)

**File:** `src/client/sound.cpp:42–177`

**Problem:**
```cpp
// soundnew() allocates:
s->ss = (LPDIRECTSOUNDBUFFER)malloc(...);
// COM object and malloc struct have mixed cleanup

// free() function (line 159–177):
for (i = 0; i < 255; i++) {
    if (tempsound[i]->ss) {
        tempsound[i]->ss->Release();  // Release COM
    }
    free((void*)tempsound[i]);  // Free struct
    // ^ BUT loop continues and may access tempsound[i] after free!
}
```

**Fix:** Separate the loops or build a free-list to avoid iterator invalidation.

---

### 6. DirectMusic Double-Release (MM-P5)

**File:** `src/client/dmusic.cpp:688–711` (destructor)

**Problem:**
```cpp
if (m_pMusicPort) {
    m_pPerformance->RemovePort(m_pMusicPort);
    m_pMusicPort->Release();  // Release at line 705
}
if (m_pMusicPort) {
    m_pMusicPort->Release();  // Release AGAIN at line 708!
}
```

**Fix:** Release only once; guard with null-check; release port before performance.

---

### 7. Lighting Array Allocation (MM-P6)

**File:** `src/client/viewport.cpp:185–193`

**Problem:**
```cpp
lighting[0] = (char*) malloc(size);
lighting[1] = (char*) malloc(size);
lighting[2] = (char*) malloc(size);
lighting[3] = (char*) malloc(size);
lighting[4] = (char*) malloc(size);
// If malloc #3 fails, #1–2 leak!
```

**Fix:** Allocate into a temp array, validate all succeed, then install.

---

## Low-Impact / Future (MM-P7, MM-P8)

### 8. Missing Global Cleanup

**File:** `src/common/u6o7.cpp` (WinMain/main exit paths)

**Problem:** No comprehensive checklist of resources to release on exit.

**Fix:** Add structured shutdown with documented cleanup steps.

---

## Verification Checklist

After each fix, verify:

- [ ] Game launches without crash
- [ ] Memory profile is flat over 10 minutes (no growth spike)
- [ ] No visual corruption or audio glitches
- [ ] Connect/disconnect/reconnect works (especially MM-P4)
- [ ] Frame rate is stable or improved (especially MM-P3)

---

## Execution Priority

**Phase MM-P1:** Create memory profile baseline
**Phase MM-P2:** DirectDraw surface fixes (highest ROI)
**Phase MM-P3:** Font leak + remove `-l` (high ROI + user-visible improvement)
**Phase MM-P4:** Socket cleanup (prevents leak on repeated connects)
**Phases MM-P5–MM-P8:** Incremental modernization


