/*
+------------------------------------------------------------------------------+
|File: framebuffer.h                                                           |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_FRAMEBUFFER_H__
#define __WEND_FRAMEBUFFER_H__

namespace Render
{
  struct Framebuffer
  {
    int width;
    int height;
    void* bitmap;
  };

  void ResizeFramebuffer(Framebuffer*, int, int);
  void RenderGradient(Render::Framebuffer* buffer, int xOffset, int yOffset);
}

#endif //__WEND_FRAMEBUFFER_H__