/*
+------------------------------------------------------------------------------+
|File: wend_win32.cpp                                                          |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

/*
  TODO: Windows-specific tasks remaining
  - File I/O
    - Saved game path
    - Asset path
  - Get a handle to our own exe file
  - Threading support
  - Mulitple keyboards support (Raw Input)
  - CPU management on battery
  - Multimonitor support
  - Fullscreen support
  - Control cursor visibility (WM_SETCURSOR)
  - Focus/unfocus (WM_ACTIVATEAPP)
  - Check blit speed
  - Hardware acceleration
  - International keyboard support (GetKeyboardLayout)
  - Windows XP controller support
 */

/* 
   TODO: Isolate and refactor platform independent code
   - App - Should control size, 
   - Keyboard - event system only
   - Mouse and event system
   - Gamepad and event system
   - Framebuffer alloc - App should request to Win32 layer
   - Soundbuffer alloc - App should request to Win32 layer
*/

// TODO: Collapse boilerplate into functions

#include "./platform/windows/wend_winapi.h"

/**
 * Windows GUI entrypoint.
 * 
 * @param instance A handle to the program's instance.
 * @param prevInstance UNUSED.
 * @param cmdLine UNUSED.
 * @param cmdShow Specifies how the window should be displayed. Used only when required.
 */
int WINAPI WinMain(HINSTANCE instance, 
                  HINSTANCE prevInstance, 
                  PSTR cmdLine,
                  int cmdShow)
{
  
  Win32::AppState* appState = (Win32::AppState*)malloc(sizeof(Win32::AppState));
  App::InitApplication(&(appState->app));
  
  int width = appState->app.width;
  int height = appState->app.height;

  appState->bitmapInfo.bmiHeader.biSize = sizeof(appState->bitmapInfo.bmiHeader);
  appState->bitmapInfo.bmiHeader.biPlanes = 1;
  appState->bitmapInfo.bmiHeader.biBitCount = 32;
  appState->bitmapInfo.bmiHeader.biCompression = BI_RGB;

  appState->app.soundBuffer.samples = (int16 *)VirtualAlloc(
    0, 
    appState->app.soundCfg.bufferSize, 
    MEM_RESERVE | MEM_COMMIT, 
    PAGE_READWRITE
  );
  
  Win32::OnResize(appState, width, height);
  
  // Register window class.
  const char CLASS_NAME[] = "Wend Class";

  WNDCLASSA windowClass = {};
  windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = Win32::WindowProc;
  windowClass.hInstance = instance;
  windowClass.lpszClassName = CLASS_NAME;

  if (!RegisterClassA(&windowClass))
  {
    return 0;
  }
  
  // Create window.
  DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
  RECT windowRect = {0, 0, width, height};
  AdjustWindowRectEx(&windowRect, dwStyle, FALSE, 0);

  HWND window = CreateWindowExA(
      0,
      CLASS_NAME,
      "Wend Engine",
      dwStyle,
      0,
      0,
      windowRect.right - windowRect.left,
      windowRect.bottom - windowRect.top,
      NULL,
      NULL,
      instance,
      appState
  );

  if (window == NULL)
  {
    return 0;
  }

  IDirectSoundBuffer* directSoundBuffer = {}; 
  Win32::InitDirectSoundBuffer(window, &directSoundBuffer, &(appState->app.soundCfg));
  Win32::ClearDirectSoundBuffer(directSoundBuffer, &(appState->app.soundCfg), 0, appState->app.soundCfg.bufferSize);
  directSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);

  fn_XInputGetState* XInputGetState = nullptr;
  fn_XInputSetState* XInputSetState = nullptr;
  Win32::InitXInput(&XInputGetState, &XInputSetState);
  if (!XInputGetState || !XInputSetState)
  {
    // TODO: Log xinput dll not found
  }

  ShowWindow(window, cmdShow);
  UpdateWindow(window);

  LARGE_INTEGER counterFrequency;
  QueryPerformanceFrequency(&counterFrequency);

  LARGE_INTEGER currentCounter = {0};
  LARGE_INTEGER lastCounter = {0};
  QueryPerformanceCounter(&currentCounter);
  lastCounter.QuadPart = currentCounter.QuadPart;

  float32 deltaTime = 0.0f;

  while (appState->app.isRunning)
  {
    MSG message = {};
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    PoolGamepadInput(XInputGetState, appState);

    DWORD lockCursor = 0;
    DWORD bytesToWrite = 0;

    Win32::GetDirectSoundState(directSoundBuffer, &(appState->app.soundBuffer), &(appState->app.soundCfg), &lockCursor, &bytesToWrite);

    App::FrameUpdate(&(appState->app), deltaTime);
    
    Win32::FillDirectSoundBuffer(directSoundBuffer, &(appState->app.soundBuffer), &(appState->app.soundCfg), lockCursor, bytesToWrite);

    HDC deviceContext = GetDC(window);
    Win32::BlitBuffer(deviceContext, window, appState);
    ReleaseDC(window, deviceContext);

    QueryPerformanceCounter(&currentCounter);
    int64 counterElapsed = currentCounter.QuadPart - lastCounter.QuadPart;
    deltaTime = (float32)counterElapsed / counterFrequency.QuadPart;

    lastCounter.QuadPart = currentCounter.QuadPart;
  }

  if (appState) 
  { 
    if (appState->app.frameBuffer.bitmap)
    {
      VirtualFree(appState->app.frameBuffer.bitmap, 0, MEM_RELEASE);
    }
    if (appState->app.soundBuffer.samples)
    {
      VirtualFree(appState->app.soundBuffer.samples, 0, MEM_RELEASE);
    }
    free(appState);
    appState = NULL;
  }
  return 0;
}