#ifndef __WEND_APPLICATION_H__
#define __WEND_APPLICATION_H__

#include <map>
#include <windows.h>

#include "../framebuffer/framebuffer.h"
#include "../input/input.h"

namespace App
{
  struct Application
  {
    Input::Keyboard keyboard;

    // Rendering
    render::Framebuffer buffer;
    BITMAPINFO bitmapInfo;

    // State
    bool isRunning;
  };

  Application* InitApplication(int, int);
}


#endif //__WEND_APPLICATION_H__