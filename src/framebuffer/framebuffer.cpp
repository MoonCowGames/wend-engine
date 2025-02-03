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

namespace Render
{
  /**
   * Changes size allocated in memory for the framebuffer bitmap when the window resizes.
   * 
   * @param buffer The framebuffer struct containing the bitmap to be resized.
   * @param width The new width of the window.
   * @param height The new height of the window.
   */
  void ResizeFramebuffer(Framebuffer* buffer, int16 width, int16 height)
  {
    if (buffer->bitmap)
    {
      VirtualFree(buffer->bitmap, 0, MEM_RELEASE);
    }

    buffer->width = width;
    buffer->height = height;

    const int8 bytesPerPixel = 4;
    int32 bitmapSize = (width * height) * bytesPerPixel;
    
    buffer->bitmap = VirtualAlloc(0, bitmapSize, MEM_COMMIT, PAGE_READWRITE);
  }
  
  // NOTE: This will change or be removed. No docstring needed.
  void RenderGradient(Render::Framebuffer* buffer, int32 xOffset, int32 yOffset)
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
}