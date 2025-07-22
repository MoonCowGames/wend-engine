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
   - Keyboard - event system only
   - Mouse - event system only
   - Gamepad - event system only
   - Framebuffer alloc - App should request to Win32 layer
   - Soundbuffer alloc - App should request to Win32 layer
*/

#include "./platform/windows/wend_winapi.h"


    /// @brief GUI entrypoint.
    /// @param instance A handle to the program's instance.
    /// @param prevInstance UNUSED.
    /// @param cmdLine UNUSED.
    /// @param cmdShow Specifies how the window should be displayed. Used only when required.
int WINAPI WinMain(HINSTANCE instance, 
                  HINSTANCE prevInstance, 
                  PSTR cmdLine,
                  int cmdShow)
{
  Win32::AppState appState;
  App::InitApplication(&(appState.app));
  
  // NOTE: This has no use until we know how to get memory out of the arena
  /* 
  Win32::MemoryArena memArena = {0};
  memArena.permanentSize = MEGABYTE(64);
  memArena.transientSize = MEGABYTE(512);

  CreateMemoryArena(&memArena); 
  */

  int width = appState.app.width;
  int height = appState.app.height;

  Win32::InitBitmapHeader(&(appState.bitmapInfo.bmiHeader));

  // TODO: Move to request from app / arena
  appState.app.soundBuffer.samples = (int16 *)VirtualAlloc(
    0, 
    appState.app.soundCfg.bufferSize, 
    MEM_RESERVE | MEM_COMMIT, 
    PAGE_READWRITE
  );
  
  Win32::OnResize(&appState, width, height);
  
  const char CLASS_NAME[] = "Wend Class";
  WNDCLASSA windowClass = {0};
  if (!Win32::RegisterWindowClass(&windowClass, instance, CLASS_NAME))
  {
    // TODO: Log/Handle fail
    return 0;
  }
  HWND window = {0};
  if (!Win32::CreateWin32Window(&window, &appState, instance, CLASS_NAME))
  {
    // TODO: Log/Handle fail
    return 0;
  }

  IDirectSoundBuffer* directSoundBuffer = {0}; 
  Win32::InitDirectSoundBuffer(window, &directSoundBuffer, &(appState.app.soundCfg));
  Win32::ClearDirectSoundBuffer(directSoundBuffer, &(appState.app.soundCfg), 0, appState.app.soundCfg.bufferSize);
  directSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);

  fn_XInputGetState* XInputGetState = nullptr;
  fn_XInputSetState* XInputSetState = nullptr;
  Win32::InitXInput(&XInputGetState, &XInputSetState);
  if (!XInputGetState || !XInputSetState)
  {
    // TODO: Log/Handle xinput dll not found
  }

  ShowWindow(window, cmdShow);
  UpdateWindow(window);
  
  float32 deltaTime = 0.0f;

  LARGE_INTEGER counterFrequency = {0};
  LARGE_INTEGER currentCounter = {0};
  LARGE_INTEGER lastCounter = {0};
  QueryPerformanceFrequency(&counterFrequency);
  QueryPerformanceCounter(&currentCounter);
  lastCounter.QuadPart = currentCounter.QuadPart;

  while (appState.app.isRunning)
  {
    MSG message = {};
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    Win32::PoolGamepadInput(XInputGetState, &appState);

    DWORD lockCursor = 0;
    DWORD bytesToWrite = 0;

    Win32::GetDirectSoundState(directSoundBuffer, &(appState.app.soundBuffer), &(appState.app.soundCfg), &lockCursor, &bytesToWrite);

    App::FrameUpdate(&(appState.app), deltaTime);
    
    Win32::FillDirectSoundBuffer(directSoundBuffer, &(appState.app.soundBuffer), &(appState.app.soundCfg), lockCursor, bytesToWrite);

    HDC deviceContext = GetDC(window);
    Win32::BlitBuffer(deviceContext, window, &appState);
    ReleaseDC(window, deviceContext);

    QueryPerformanceCounter(&currentCounter);
    int64 counterElapsed = currentCounter.QuadPart - lastCounter.QuadPart;
    deltaTime = (float32)counterElapsed / counterFrequency.QuadPart;

    lastCounter.QuadPart = currentCounter.QuadPart;
  }

  // Clean up
  if (appState.app.frameBuffer.bitmap)
  {
    VirtualFree(appState.app.frameBuffer.bitmap, 0, MEM_RELEASE);
  }
  if (appState.app.soundBuffer.samples)
  {
    VirtualFree(appState.app.soundBuffer.samples, 0, MEM_RELEASE);
  }
  return 0;
}