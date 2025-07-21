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
#include "../sound/sound.h"
#include "../misc/includes.h"

namespace App
{
  /// @struct Application Manages state of the platform-independent application
  struct Application
  {
    // Window
    int32 xPos = 0;
    int32 yPos = 0;
    int32 width = 0;
    int32 height = 0;

    // Input
    Input::Keyboard keyboard = {0};
    Input::Mouse mouse = {0};
    Input::Gamepad gamepad[4] = {0};

    // Rendering
    Render::Framebuffer frameBuffer = {0};

    // TODO: Add Audio service
    Sound::Buffer soundBuffer = {0};
    Sound::Configuration soundCfg = {0};

    // State
    bool isRunning = false;
  };

      /**
       * Allocates and creates an instance of an Application struct.
       * 
       * @param width Width of window in pixels.
       * @param height Height of window in pixels.
       * @return Returns pointer to the application being initialised.
       */
  void InitApplication(Application* app);

      /**
       * Processes per-frame changes of entities.
       * 
       * @param deltaTime The time in seconds between the last two frames
       */
  void FrameUpdate(Application* app, float32 deltaTime);
}

#endif //__WEND_APPLICATION_H__