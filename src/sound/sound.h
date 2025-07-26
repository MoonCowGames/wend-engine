/*
+------------------------------------------------------------------------------+
|File: sound.h                                                                 |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_SOUND_H__
#define __WEND_SOUND_H__

#include <math.h>
#include <windows.h>
#include <dsound.h>

#include "../misc/includes.h"

#define PI32 3.14159265358979323846f

namespace Sound
{
      /// @struct Configuration Manages state of the platform-independent audio buffer
  struct Configuration
  {
    int32 samplesPerSecond = 0;
    int32 frequency = 0;
    int32 volume = 0;
    int32 wavePeriod = 0;
    int32 bytesPerSample = 0;
    int32 bufferSize = 0;
    uint32 runningSampleIndex = 0;
  };

  struct Buffer
  {
    int16 *samples = 0;
    int32 sampleCount = 0;
  };

  void InitSoundBuffer(
      Sound::Buffer *soundBuffer, 
      Sound::Configuration *soundCfg);

  void FillSoundBuffer(
      Sound::Buffer *soundBuffer, 
      Sound::Configuration *soundCfg);
}

#endif //__WEND_SOUND_H__