/*
+------------------------------------------------------------------------------+
|File: framebuffer.cpp                                                         |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include <windows.h>
#include <cstdint>

#include "framebuffer.h"

// NOTE: This will change or be removed. No docstring needed.
void Render::RenderGradient(Render::Framebuffer* buffer, int32 xOffset, int32 yOffset)
{
  int pitch = buffer->width*4;
  uint8* row = (uint8*)buffer->bitmap;
  for(int y = 0; y < buffer->height; ++y)
  {
    uint32* pixel = (uint32*)row;
    for(int x = 0; x < buffer->width; ++x)
    {
      *pixel = (uint8)(x+xOffset) << 16 | 
              (uint8)(y+yOffset) << 8 | 
              255;
      ++pixel;
    }
    row += pitch;
  }
}