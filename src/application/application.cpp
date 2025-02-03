#include "application.h"

namespace App
{
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

  void FrameUpdate(float32 deltaTime)
  {
    // TODO: Process per-frame changes.
  }

  // Win32
 
}