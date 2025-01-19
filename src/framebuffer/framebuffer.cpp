#include <windows.h>
#include <cstdint>

#include "framebuffer.h"

namespace Render
{
  void ResizeFramebuffer(Framebuffer* buffer, int width, int height)
  {
    if (buffer->bitmap)
    {
      VirtualFree(buffer->bitmap, 0, MEM_RELEASE);
    }

    buffer->width = width;
    buffer->height = height;

    const int bytesPerPixel = 4;
    int bitmapSize = (width * height) * bytesPerPixel;
    
    buffer->bitmap = VirtualAlloc(0, bitmapSize, MEM_COMMIT, PAGE_READWRITE);
  }

  void RenderGradient(Render::Framebuffer* buffer, int xOffset, int yOffset)
  {
    int pitch = buffer->width*4;
    uint8_t* row = (uint8_t*)buffer->bitmap;
    for(int y = 0; y < buffer->height; ++y)
    {
      uint32_t* pixel = (uint32_t*)row;
      for(int x = 0; x < buffer->width; ++x)
      {
        *pixel = (uint8_t)(x+xOffset) << 16 | 
                (uint8_t)(y+yOffset) << 8 | 
                255;
        ++pixel;
      }
      row += pitch;
    }
  }
}