/*
+------------------------------------------------------------------------------+
|File: audio.cpp                                                               |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "sound.h"

int16 SineWave(float32 time, int32 volume)
{
  return (int16)(sinf(time)*(float32)volume);
}