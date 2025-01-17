#ifndef __WEND_FRAMEBUFFER_H__
#define __WEND_FRAMEBUFFER_H__

namespace render
{
  struct Framebuffer
  {
    int width;
    int height;
    void* bitmap;
  };

  void ResizeFramebuffer(Framebuffer*, int, int);
}

#endif //__WEND_FRAMEBUFFER_H__