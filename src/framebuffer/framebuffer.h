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
      /// @struct Framebuffer Manages state of the platform-independent framebuffer
  struct Framebuffer
  {
    void* bitmap = 0;
    int16 width = 0;
    int16 height = 0;
  };
  
      // NOTE: This will change or be removed. No docstring needed.
  void RenderGradient(
    Render::Framebuffer* frameBuffer, 
    int32 xOffset, 
    int32 yOffset);
}

#endif //__WEND_FRAMEBUFFER_H__