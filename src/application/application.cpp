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
  Application* InitApplication(int width, int height)
  {
    Application* app = (Application*)malloc(sizeof(Application));
    app->isRunning = true;

    app->bitmapInfo.bmiHeader.biSize = sizeof(app->bitmapInfo.bmiHeader);
    app->bitmapInfo.bmiHeader.biPlanes = 1;
    app->bitmapInfo.bmiHeader.biBitCount = 32;
    app->bitmapInfo.bmiHeader.biCompression = BI_RGB;
    app->bitmapInfo.bmiHeader.biWidth = width;
    app->bitmapInfo.bmiHeader.biHeight = -height;

    app->buffer.width = width;
    app->buffer.height = height;
    Render::ResizeFramebuffer(&(app->buffer), width, height);
    
    for (int index = 0; index < 256; index++)
    {
      app->keyboard.keyState[index] = 0;
    }

    return app;
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