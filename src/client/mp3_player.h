#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include "music_player.h"

class CMp3Music : public IMusicPlayer {
    class Impl;
    Impl* pImpl;
public:
    CMp3Music();
    virtual ~CMp3Music();
    HRESULT Initialize() override;
    HRESULT LoadMidiFromFile(const char* path, BOOL bMidiFile) override;
    HRESULT Play() override;
    HRESULT Stop() override;
    HRESULT IsPlaying() override;
    void SetMasterVolume(long volume) override;
};

#endif
