/*
+------------------------------------------------------------------------------+
|File: win_main.h                                                              |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/
#ifndef __WEND_WIN_MAIN_H__
#define __WEND_WIN_MAIN_H__

#include <windows.h>
#include <xinput.h>

#include "framebuffer/framebuffer.h"
#include "application/application.h"
#include "input/input.h"
#include "audio/audio.h"
#include "misc/includes.h"

#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef XINPUT_GET_STATE(fn_XInputGetState);

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef XINPUT_SET_STATE(fn_XInputSetState);

namespace Win32
{
  struct AppState
  {
    App::Application app;
    BITMAPINFO bitmapInfo;
  };
  
  void BlitBuffer(HDC deviceContext, HWND window, AppState* appState);
  void InitXInput(fn_XInputGetState**, fn_XInputSetState**);
}

#endif //__WEND_WIN_MAIN_H__