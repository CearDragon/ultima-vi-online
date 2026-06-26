#include "stdafx.h"
#include "resource.h"
#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <math.h>

#include "myfile.h"
LPDIRECTSOUND dsnd;
extern HWND hWnd;
extern bool u6o_sound;
extern unsigned char u6ovolume;
bool DirectSoundCreate_fail = FALSE;

// MM-P9 diagnostic (2026-06-25): DirectSound voice-ring counters, surfaced by
// the txtout() U6O-DIAG heartbeat. g_snd_dup_n is cumulative DuplicateSoundBuffer
// calls (sfx fire rate); g_snd_live is the current count of live tempsound[]
// voices (must stay bounded <=256). Lets the next run confirm the sfx ring is
// NOT the in-game accelerator. Remove with the rest of the MM-P9 instrumentation.
long g_snd_dup_n = 0;
long g_snd_live = 0;

struct sound {
    DSBUFFERDESC d;
    WAVEFORMATEX wf;
    LPDIRECTSOUNDBUFFER s;
    unsigned char *o;
    sound *ss;
};

sound *tempsound[256];

bool soundsetupf = FALSE;

bool soundsetup() {
    if (DS_OK != DirectSoundCreate(NULL, &dsnd, NULL)) {
        DirectSoundCreate_fail = TRUE;
        MessageBox(NULL, "WAV DirectSoundCreate failed", "Ultima 6 Online", MB_OK);
        return FALSE;
    }
    dsnd->SetCooperativeLevel(hWnd, DSSCL_NORMAL);
    static long i;
    for (i = 0; i < 256; i++) {
        tempsound[i] = NULL;
    }
    soundsetupf = TRUE;
    return TRUE;
}

sound *soundnew(long freq, long bit, long stereo, long bytes) {
    if (DirectSoundCreate_fail) return NULL;
    if (soundsetupf == FALSE) soundsetup();
    if (DirectSoundCreate_fail) return NULL;
    sound *ts = (sound *) malloc(sizeof(sound));
    ZeroMemory(ts, sizeof(sound));
    ts->wf.cbSize = 0;
    ts->wf.wFormatTag = WAVE_FORMAT_PCM;
    ts->wf.nChannels = stereo;
    ts->wf.nSamplesPerSec = freq;
    ts->wf.wBitsPerSample = bit;
    ts->wf.nBlockAlign = ts->wf.nChannels * ts->wf.wBitsPerSample / 8;
    ts->wf.nAvgBytesPerSec = ts->wf.nSamplesPerSec * ts->wf.nBlockAlign;
    ts->d.dwSize = sizeof(DSBUFFERDESC);
    //software buffer is used to maintain offset of sound data
    ts->d.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY |
                    DSBCAPS_GETCURRENTPOSITION2;
    ts->d.dwBufferBytes = bytes;
    ts->d.lpwfxFormat = &ts->wf;
    dsnd->CreateSoundBuffer(&ts->d, &ts->s, NULL);
    ts->d.dwBufferBytes = bytes;
    static void *o1;
    static unsigned long s1;
    ts->s->Lock(0, 0, &o1, &s1, NULL, NULL, DSBLOCK_ENTIREBUFFER);
    ts->o = (unsigned char *) o1;
    ts->s->Unlock(o1, s1, NULL, NULL);
    return ts;
}

sound *soundload(LPCSTR fn) {
    if (DirectSoundCreate_fail) return NULL;
    if (soundsetupf == FALSE) soundsetup();
    if (DirectSoundCreate_fail) return NULL;
    static sound *ts;
    static file *fh;
    static long freq, bits, stereo, x;
    fh = open(fn);
    if (fh->h == HFILE_ERROR) {
        ts = NULL;
        return ts;
    }
    seek(fh, 24);
    get(fh, &freq, 4);
    seek(fh, 34);
    bits = 0;
    get(fh, &bits, 2);
    seek(fh, 32);
    stereo = 0;
    get(fh, &stereo, 2);
    if (stereo == (bits / 8)) { stereo = 1; } else { stereo = 2; }
    ts = soundnew(freq, bits, stereo, lof(fh) - 58 - 16 - 16 - 32 - 32);
    seek(fh, 58);
    get(fh, ts->o, lof(fh) - 58 - 16 - 16 - 32 - 32);
    close(fh);
    return ts;
}

sound *soundplay(sound *s) {
    if (DirectSoundCreate_fail) return NULL;
    //if (u6o_sound==FALSE) return NULL; //no sound
    if (soundsetupf == FALSE) return NULL;
    static unsigned long i, i2;
    if (s == NULL) return NULL;
    i = -1;
busysound:
    i++;
    if (i == 256) return NULL;
    if (tempsound[i] != NULL) {
        tempsound[i]->s->GetStatus(&i2);
        if (i2 & DSBSTATUS_PLAYING) goto busysound;
        tempsound[i]->s->Release();
        free((void *) tempsound[i]);
        g_snd_live--; // MM-P9 diagnostic: finished voice reclaimed.
    }
    sound *ts = (sound *) malloc(sizeof(sound));
    memcpy(ts, s, sizeof(sound));
    dsnd->DuplicateSoundBuffer(s->s, &ts->s);
    g_snd_dup_n++; // MM-P9 diagnostic: count voice duplicates.
    g_snd_live++;
    tempsound[i] = ts;
    tempsound[i]->ss = s;
    ts->s->Play(NULL, NULL, NULL);
    return ts;
}

sound *soundplay2(sound *s, long v) {
    if (DirectSoundCreate_fail) return NULL;
    if (soundsetupf == FALSE) return NULL;
    if (u6ovolume == 0) return NULL;
    if (v > 255) v = 255;
    if (v <= 0) return NULL;
    static long i;
    static unsigned long i2;
    static float f;
    if (s == NULL) return NULL;
    i = -1;
busysound2:
    i++;
    if (i == 256) return NULL;
    if (tempsound[i] != NULL) {
        tempsound[i]->s->GetStatus(&i2);
        if (i2 & DSBSTATUS_PLAYING) goto busysound2;
        tempsound[i]->s->Release();
        free((void *) tempsound[i]);
        g_snd_live--; // MM-P9 diagnostic: finished voice reclaimed.
    }
    sound *ts = (sound *) malloc(sizeof(sound));
    memcpy(ts, s, sizeof(sound));
    dsnd->DuplicateSoundBuffer(s->s, &ts->s);
    g_snd_dup_n++; // MM-P9 diagnostic: count voice duplicates.
    g_snd_live++;
    tempsound[i] = ts;
    tempsound[i]->ss = s;
    f = v;
    f = f * (float) u6ovolume / 255.0f;
    f = 255 - f;
    f = f * 0.25f;
    f *= f;
    ts->s->SetVolume(-f);
    ts->s->Play(NULL, NULL, NULL);
    return ts;
}

void free(sound *s) {
    if (DirectSoundCreate_fail) return;
    if (soundsetupf == FALSE) return;
    if (s != NULL) {
        if (s->s) s->s->Release();
        static long i;
        for (i = 0; i < 256; i++) {
            if (tempsound[i] != NULL) {
                if (tempsound[i]->ss == s) {
                    if (tempsound[i]->s) tempsound[i]->s->Release();
                    free((void *) tempsound[i]);
                    tempsound[i] = NULL;
                }
            }
        }
        free((void *) s);
    }
    return;
}

// RW shutdown-fix (2026-05-28): fast best-effort teardown of every live
// DirectSound voice + the global dsnd device. Called from the client
// shutdown path in u6o7.cpp so dsound.dll releases its hold on client.exe
// (and any in-flight WAV stops audibly) before ExitProcess runs. Safe when
// sound setup never completed -- guards on per-entry and global pointers.
//
// MM-P8.1: RAII candidate — the tempsound[] voice pool + the global dsnd
// device form a self-contained "SoundManager" subsystem. Wrapping each voice
// in a small RAII type (Stop()+Release() in its dtor) and the device in a
// ComPtr would let pool teardown happen automatically and remove this manual
// Stop/Release/free triad.
void soundshutdown() {
    if (soundsetupf == FALSE) return;
    for (long i = 0; i < 256; i++) {
        if (tempsound[i] != NULL) {
            if (tempsound[i]->s) {
                tempsound[i]->s->Stop();
                tempsound[i]->s->Release();
            }
            free((void *) tempsound[i]);
            tempsound[i] = NULL;
        }
    }
    if (dsnd) {
        dsnd->Release();
        dsnd = NULL;
    }
    soundsetupf = FALSE;
}