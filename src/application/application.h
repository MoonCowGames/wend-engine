/*
+------------------------------------------------------------------------------+
|File: application.h                                                           |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_APPLICATION_H__
#define __WEND_APPLICATION_H__

#include <windows.h>

#include "../framebuffer/framebuffer.h"
#include "../input/input.h"
#include "../misc/includes.h"

namespace App
{
  struct Application
  {
    // Input
    Input::Keyboard keyboard;

    // Rendering
    Render::Framebuffer buffer;

    // TODO: Add Audio service

    // State
    bool isRunning;
  };

  void InitApplication(Application*);
  void FrameUpdate(float32 deltaTime);
}

#endif //__WEND_APPLICATION_H__