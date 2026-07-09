#include "stdafx.h"
#include <mmsystem.h>
#include "mp3_player.h"
#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include <dsound.h>
#include <vector>

extern LPDIRECTSOUND dsnd;

struct CMp3Music::Impl {
    LPDIRECTSOUNDBUFFER m_pBuffer = nullptr;
    long m_volume = 0;
};

CMp3Music::CMp3Music() : pImpl(new Impl()) {}
CMp3Music::~CMp3Music() { Stop(); if (pImpl->m_pBuffer) pImpl->m_pBuffer->Release(); delete pImpl; }

HRESULT CMp3Music::Initialize() { return S_OK; } // dsnd already init by soundsetup()

HRESULT CMp3Music::LoadMidiFromFile(const char* path, BOOL bMidiFile) {
    Stop();
    if (pImpl->m_pBuffer) { pImpl->m_pBuffer->Release(); pImpl->m_pBuffer = nullptr; }

    mp3dec_t mp3d;
    mp3dec_file_info_t info;
    memset(&info, 0, sizeof(info));
    if (mp3dec_load(&mp3d, path, &info, NULL, NULL) != 0) return E_FAIL;

    WAVEFORMATEX wf = { WAVE_FORMAT_PCM, (WORD)info.channels, (DWORD)info.hz, 
                       (DWORD)info.hz * info.channels * 2, (WORD)(info.channels * 2), 16, 0 };
    DSBUFFERDESC d = { sizeof(DSBUFFERDESC), DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME, 
                      (DWORD)(info.samples * sizeof(mp3d_sample_t)), 0, &wf };

    if (dsnd->CreateSoundBuffer(&d, &pImpl->m_pBuffer, NULL) == DS_OK) {
        void* ptr; DWORD bytes;
        pImpl->m_pBuffer->Lock(0, 0, &ptr, &bytes, NULL, NULL, DSBLOCK_ENTIREBUFFER);
        memcpy(ptr, info.buffer, bytes);
        pImpl->m_pBuffer->Unlock(ptr, bytes, NULL, NULL);
    }
    free(info.buffer);
    if (pImpl->m_pBuffer) pImpl->m_pBuffer->SetVolume(pImpl->m_volume);
    return pImpl->m_pBuffer ? S_OK : E_FAIL;
}

HRESULT CMp3Music::Play() { 
    if (!pImpl->m_pBuffer) return E_FAIL;
    pImpl->m_pBuffer->SetCurrentPosition(0);
    return pImpl->m_pBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

HRESULT CMp3Music::Stop() { if (pImpl->m_pBuffer) pImpl->m_pBuffer->Stop(); return S_OK; }

HRESULT CMp3Music::IsPlaying() {
    static unsigned char safety_toggle = 0;
    safety_toggle = 1 - safety_toggle;
    extern unsigned char u6omusicsetup;
    if (u6omusicsetup == 0) return (HRESULT)safety_toggle;

    if (!pImpl->m_pBuffer) return (HRESULT)safety_toggle;

    DWORD status;
    if (FAILED(pImpl->m_pBuffer->GetStatus(&status))) return (HRESULT)safety_toggle;
    return (status & DSBSTATUS_PLAYING) ? S_OK : S_FALSE;
}

void CMp3Music::SetMasterVolume(long volume) {
    pImpl->m_volume = volume;
    if (pImpl->m_pBuffer) pImpl->m_pBuffer->SetVolume(pImpl->m_volume);
}
