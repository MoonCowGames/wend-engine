/*
+------------------------------------------------------------------------------+
|File: audio.h                                                                 |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_AUDIO_H__
#define __WEND_AUDIO_H__

#include <math.h>
#include <windows.h>
#include <dsound.h>

#include "../misc/includes.h"

#define PI32 3.14159265358979323846f

namespace Audio
{
  struct Configuration
  {
    int32 samplesPerSecond;
    int32 frequency;
    int32 volume;
    int32 wavePeriod;
    int32 bytesPerSample;
    int32 bufferSize;
    uint32 runningSampleIndex;
  };

  void InitDirectSound(IDirectSoundBuffer**, HWND, Configuration);
  void TestAudioBuffer(IDirectSoundBuffer*, Configuration*);
  void FillBuffer(IDirectSoundBuffer*, Configuration*, DWORD, DWORD);
  int16 SineWave(float32, int32);
}

#endif //__WEND_AUDIO_H__