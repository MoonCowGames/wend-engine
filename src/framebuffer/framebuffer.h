/*
+------------------------------------------------------------------------------+
|File: framebuffer.h                                                           |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_FRAMEBUFFER_H__
#define __WEND_FRAMEBUFFER_H__

#include "../misc/includes.h"

namespace Render
{
  struct Framebuffer
  {
    int16 width;
    int16 height;
    void* bitmap;
  };

  void ResizeFramebuffer(Framebuffer*, int16, int16);
  void RenderGradient(Render::Framebuffer* buffer, int32 xOffset, int32 yOffset);
}

#endif //__WEND_FRAMEBUFFER_H__