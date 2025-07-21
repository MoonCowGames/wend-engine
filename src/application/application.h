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
    // Input
    Input::Keyboard keyboard;
    Input::Mouse mouse;
    Input::Gamepad gamepad[4];

    // Rendering
    Render::Framebuffer frameBuffer;

    // TODO: Add Audio service
    Sound::Buffer soundBuffer;
    Sound::Configuration soundCfg;

    // State
    bool isRunning;
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