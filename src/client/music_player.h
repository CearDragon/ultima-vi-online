#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#ifdef _WIN32
#include "stdafx.h"
#endif

// Infoport structure
typedef struct INFOPORT {
    char szPortDescription[64 * 2];
    DWORD dwFlags;
    DWORD dwClass;
    DWORD dwType;
    DWORD dwMaxAudioChannels;
    DWORD dwMaxVoices;
    DWORD dwMaxChannelGroups;
    DWORD dwEffectFlags;
    GUID guidSynthGUID;
} *LPINFOPORT;

class IMusicPlayer {
public:
    virtual ~IMusicPlayer() {}
    virtual HRESULT Initialize() = 0;
    virtual HRESULT PortEnumeration(DWORD dwIndex, LPINFOPORT lpInfoPort) { return E_NOTIMPL; }
    virtual HRESULT SelectPort(LPINFOPORT lpInfoPort) { return E_NOTIMPL; }
    virtual HRESULT LoadMidiFromFile(const char* path, BOOL bMidiFile) = 0;
    virtual HRESULT Play() = 0;
    virtual HRESULT Stop() = 0;
    virtual HRESULT IsPlaying() = 0; // S_OK if playing, S_FALSE if not
    virtual void SetMasterVolume(long volume) = 0; // hundredths of decibels (mB)
};

#endif
