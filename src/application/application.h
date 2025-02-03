/*
+------------------------------------------------------------------------------+
|File: application.h                                                           |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_APPLICATION_H__
#define __WEND_APPLICATION_H__

#include <map>
#include <iostream>
#include <iomanip>
#include <windows.h>

#include "../framebuffer/framebuffer.h"
#include "../input/input.h"

typedef float float32;
typedef double float64;

namespace App
{
  struct Application
  {
    Input::Keyboard keyboard;

    // Rendering
    Render::Framebuffer buffer;
    BITMAPINFO bitmapInfo;

    // State
    bool isRunning;
  };

  Application* InitApplication(int, int);
  void FrameUpdate(float32 deltaTime);
}

#endif //__WEND_APPLICATION_H__