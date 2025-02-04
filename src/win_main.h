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

namespace Win32
{
  struct AppState
  {
    App::Application app;
    BITMAPINFO bitmapInfo;
  };
  void BlitBuffer(HDC deviceContext, HWND window, AppState* appState);
}

#endif //__WEND_WIN_MAIN_H__