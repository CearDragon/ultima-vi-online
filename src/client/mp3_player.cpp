#include "stdafx.h"
#include <mmsystem.h>
#include "mp3_player.h"
#include <cmath>
#include <stdio.h>

namespace {
    constexpr const char* kMp3Alias = "u6omp3";
}

struct CMp3Music::Impl {
    bool isOpen = false;
    long m_volume = 0;
};

CMp3Music::CMp3Music() : pImpl(new Impl()) {}
CMp3Music::~CMp3Music() {
    Stop();
    mciSendStringA("close " "u6omp3", NULL, 0, NULL);
    pImpl->isOpen = false;
    delete pImpl;
}

HRESULT CMp3Music::Initialize() {
    mciSendStringA("close " "u6omp3", NULL, 0, NULL);
    pImpl->isOpen = false;
    return S_OK;
}

HRESULT CMp3Music::LoadMidiFromFile(const char* path, BOOL bMidiFile) {
    (void)bMidiFile;
    if (path == NULL || path[0] == 0) return E_INVALIDARG;

    Stop();
    if (pImpl->isOpen) {
        mciSendStringA("close " "u6omp3", NULL, 0, NULL);
        pImpl->isOpen = false;
    }

    char command[1024];
    sprintf_s(command, sizeof(command), "open \"%s\" type mpegvideo alias %s", path, kMp3Alias);
    if (mciSendStringA(command, NULL, 0, NULL) != 0) {
        return E_FAIL;
    }

    pImpl->isOpen = true;
    SetMasterVolume(pImpl->m_volume);
    return S_OK;
}

HRESULT CMp3Music::Play() {
    if (!pImpl->isOpen) return E_FAIL;
    if (mciSendStringA("play " "u6omp3" " repeat", NULL, 0, NULL) != 0) return E_FAIL;
    return S_OK;
}

HRESULT CMp3Music::Stop() {
    if (pImpl->isOpen) {
        mciSendStringA("stop " "u6omp3", NULL, 0, NULL);
    }
    return S_OK;
}

HRESULT CMp3Music::IsPlaying() {
    static unsigned char fallbackToggle = 0;
    fallbackToggle = 1 - fallbackToggle;
    extern unsigned char u6omusicsetup;
    if (u6omusicsetup == 0) return (HRESULT) fallbackToggle;

    if (!pImpl->isOpen) return (HRESULT) fallbackToggle;

    char status[64] = {0};
    if (mciSendStringA("status " "u6omp3" " mode", status, (UINT) sizeof(status), NULL) != 0) {
        return (HRESULT) fallbackToggle;
    }
    if (_stricmp(status, "playing") == 0) return S_OK;
    return S_FALSE;
}

void CMp3Music::SetMasterVolume(long volume) {
    pImpl->m_volume = volume;
    if (!pImpl->isOpen) return;

    // MCI volume range is [0, 1000]. Input is hundredths of dB (typically <= 0).
    const double amplitude = pow(10.0, (double) pImpl->m_volume / 2000.0);
    long mciVolume = (long) (amplitude * 1000.0 + 0.5);
    if (mciVolume < 0) mciVolume = 0;
    if (mciVolume > 1000) mciVolume = 1000;

    char command[128];
    sprintf_s(command, sizeof(command), "setaudio %s volume to %ld", kMp3Alias, mciVolume);
    mciSendStringA(command, NULL, 0, NULL);
}
