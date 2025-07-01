/*
+------------------------------------------------------------------------------+
|File: win_main.h                                                              |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/
#ifndef __WEND_WIN32_H__
#define __WEND_WIN32_H__

#include <windows.h>
#include <xinput.h>

#include "../../framebuffer/framebuffer.h"
#include "../../application/application.h"
#include "../../input/input.h"
#include "../../audio/audio.h"
#include "../../misc/includes.h"

#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef XINPUT_GET_STATE(fn_XInputGetState);

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef XINPUT_SET_STATE(fn_XInputSetState);

namespace Win32
{
      /**
       * @struct AppState Provides Windows with a handle to the application and associated data.
       */
  struct AppState
  {
    App::Application app;
    BITMAPINFO bitmapInfo;
  };

      /**
       * Displays the framebuffer on the window.
       * 
       * @param deviceContext A Windows structure required for displaying graphics.
       * @param window The handle to the GUI window.
       * @param buffer The framebuffer to be displayed.
       * @param bitmapInfo Information Windows requires to display the framebuffer.
       */
  void BlitBuffer(HDC deviceContext, HWND window, AppState* appState);

      /**
       * Prepares Windows' XInput library and prepares function pointers 
       * to XInput API. Allows for dynamic function loading in case end-user does
       * not have the required XInput dll installed.
       * 
       * @param XInputGetState Function pointer to load XInput function into. Used to get the current state of a controller.
       * @param XInputSetState Function pointer to load XInput function into. Used to set state of a controller for vibration.
       */
  void InitXInput(fn_XInputGetState** XInputGetState, fn_XInputSetState** XInputSetState);

      /**
       * Changes size allocated in memory for the framebuffer bitmap when the window resizes.
       * 
       * @param appState Struct containing framebuffer and BITMAPINFO
       * @param width The new width of the window.
       * @param height The new height of the window.
       */
  void OnResize(AppState* appState, int16 width, int16 height);
}

#endif //__WEND_WIN32_H__