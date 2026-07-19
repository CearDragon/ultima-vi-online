#ifndef _INC_SOUND
#define _INC_SOUND
#include <mmsystem.h>
#include <dsound.h>
#include "voiceover.h"
extern LPDIRECTSOUND dsnd;

struct sound {
    DSBUFFERDESC d;
    WAVEFORMATEX wf;
    LPDIRECTSOUNDBUFFER s;
    unsigned char *o;
    sound *ss;
};

sound *soundnew(long freq, long bit, long stereo, long bytes);

sound *soundload(LPCSTR fn, float gain = 1.0f);

sound *soundplay(sound *s);

sound *soundplay2(sound *s, long v, long global_v = -1, bool allow_boost = false);

bool soundsetup();

void free(sound *s);

// Fast-shutdown helper: stop and release every live DirectSound buffer in
// tempsound[] (best-effort, ignores errors), then release the global dsnd
// device. Used by the client shutdown path so dsound.dll stops holding a
// lock on client.exe before ExitProcess runs.
void soundshutdown();
bool sound_is_any_copy_playing(sound *s);
#endif
