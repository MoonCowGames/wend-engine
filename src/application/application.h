#ifndef __WEND_APPLICATION_H__
#define __WEND_APPLICATION_H__

#include <map>
#include <windows.h>

#include "../framebuffer/framebuffer.h"
#include "../input/input.h"

namespace App
{
  // TODO: Extract to module
  // StateInfo (consider renaming to Application) defines application state.
  // It is inteded for crossplatform usage, and will need to be extracted into its own header. 
  // We can implement Win32 specific struct using preprocessor directives.
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