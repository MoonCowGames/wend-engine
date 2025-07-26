/*
+------------------------------------------------------------------------------+
|File: audio.cpp                                                               |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "sound.h"

void Sound::InitSoundBuffer(Sound::Buffer *soundBuffer, Sound::Configuration *soundCfg)
{
  *soundBuffer = {};
  *soundCfg = {};

  soundCfg->samplesPerSecond = 48000;
  soundCfg->frequency = 261;
  soundCfg->volume = 4000;
  soundCfg->wavePeriod = soundCfg->samplesPerSecond / soundCfg->frequency;
  soundCfg->bytesPerSample = sizeof(int16)*2;
  soundCfg->bufferSize = soundCfg->samplesPerSecond * soundCfg->bytesPerSample;
  soundCfg->runningSampleIndex = 0;
}

// TODO: Fix framerate dependancy
void Sound::FillSoundBuffer(Sound::Buffer *soundBuffer, Sound::Configuration *soundCfg)
{
  static float32 time;
  int16* sample = soundBuffer->samples;
  for (int32 i = 0; i < soundBuffer->sampleCount; i++)
  {
    int16 sampleValue = (int16)(sinf(time) * 4000);
    // left
    *sample++ = sampleValue;
    // right
    *sample++ = sampleValue;
    time += 2.0f * PI32 * 1.0f / (float32)soundCfg->wavePeriod; 
  }
}