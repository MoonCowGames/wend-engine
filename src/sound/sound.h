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
    int32 samplesPerSecond;
    int32 frequency;
    int32 volume;
    int32 wavePeriod;
    int32 bytesPerSample;
    int32 bufferSize;
    uint32 runningSampleIndex;
  };

  struct Buffer
  {
    int16 *samples;
    int32 sampleCount;
  };

  void InitSoundBuffer(
      Sound::Buffer *soundBuffer, 
      Sound::Configuration *soundCfg);

  void FillSoundBuffer(
      Sound::Buffer *soundBuffer, 
      Sound::Configuration *soundCfg);
}

#endif //__WEND_SOUND_H__