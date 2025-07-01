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

      /**
       * Prepares Windows' DirectSound library and prepares sound buffers.
       * 
       * @param soundBuffer The secondary audio buffer that can be written to.
       * @param window The handle to the GUI window to bind the audio service to.
       * @param config A struct containing configuration information about the 
       * audio service, such as frequency and sample rate.
       */
  void InitDirectSound(IDirectSoundBuffer**, HWND, Configuration);
  
      /**
       * Fills sound buffer with a given audio sample.
       * TODO: Change to accepting an audio sample as parameter. Currently just uses a sine wave.
       * 
       * @param soundBuffer The secondary audio buffer that can be written to.
       * @param config A struct containing configuration information about the 
       * audio service, such as frequency and sample rate.
       * @param lockCursor Determines the point of the buffer to lock and prepare to write to.
       * @param bytesToWrite The number of bytes expected to write to.
       */
  void FillBuffer(IDirectSoundBuffer*, Configuration*, DWORD, DWORD);
  
      // NOTE: This will change or be removed. No docstring needed.
  void TestAudioBuffer(IDirectSoundBuffer*, Configuration*);
  
      // NOTE: This will change or be removed. No docstring needed.
  int16 SineWave(float32, int32);
}

#endif //__WEND_AUDIO_H__