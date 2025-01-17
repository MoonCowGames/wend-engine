//#define WIN32
#include "framebuffer.h"
  #include <windows.h>

namespace render
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
}