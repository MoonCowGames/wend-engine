#ifndef __WEND_AUDIO_H__
#define __WEND_AUDIO_H__

#include <math.h>
#include <windows.h>
#include <dsound.h>
#include <cstdint>
#include <iostream>

#define PI32 M_PI

typedef float float32;
typedef double float64;

namespace Audio
{
  struct Configuration
  {
    int32_t samplesPerSecond;
    int32_t frequency;
    int32_t volume;
    int32_t wavePeriod;
    int32_t bytesPerSample;
    int32_t bufferSize;
    uint32_t runningSampleIndex;
  };

  void InitDirectSound(IDirectSoundBuffer**, HWND, Configuration);
  void TestAudioBuffer(IDirectSoundBuffer*, Configuration*);
  void FillBuffer(IDirectSoundBuffer*, Configuration*, DWORD, DWORD);
  int16_t SineWave(float32, int32_t);
}

#endif //__WEND_AUDIO_H__