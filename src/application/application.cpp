/*
+------------------------------------------------------------------------------+
|File: application.cpp                                                         |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "application.h"

namespace App
{
  /**
   * Allocates and creates an instance of an Application struct.
   * 
   * @param width Width of window in pixels.
   * @param height Height of window in pixels.
   * @return Returns pointer to the application being initialised.
   */
  void InitApplication(Application* app)
  {
    app->isRunning = true;
    
    for (int index = 0; index < 256; index++)
    {
      app->keyboard.keyState[index] = 0;
    }
  }

  /**
   * Processes per-frame changes of entities.
   * 
   * @param deltaTime The time in seconds between the last two frames
   */
  void FrameUpdate(float32 deltaTime)
  {
    // TODO: Process per-frame changes.
  }
}