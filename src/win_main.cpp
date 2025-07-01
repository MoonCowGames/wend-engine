/*
+------------------------------------------------------------------------------+
|File: win_main.cpp                                                            |
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

#include "./platform/windows/win32.h"

LRESULT CALLBACK WindowProc(HWND window, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam);

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
  int width = 1280;
  int height = 720;

  Win32::AppState* appState = (Win32::AppState*)malloc(sizeof(Win32::AppState));
  App::InitApplication(&(appState->app));
  appState->bitmapInfo.bmiHeader.biSize = sizeof(appState->bitmapInfo.bmiHeader);
  appState->bitmapInfo.bmiHeader.biPlanes = 1;
  appState->bitmapInfo.bmiHeader.biBitCount = 32;
  appState->bitmapInfo.bmiHeader.biCompression = BI_RGB;
  
  Win32::OnResize(appState, width, height);
  
  // Register window class.
  const char CLASS_NAME[] = "Wend Class";

  WNDCLASSA windowClass = {};
  windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = WindowProc;
  windowClass.hInstance = instance;
  windowClass.lpszClassName = CLASS_NAME;

  if (!RegisterClassA(&windowClass))
  {
    return 0;
  }
  
  // Create window.

  HWND window = CreateWindowExA(
      0,
      CLASS_NAME,
      "Wend Engine",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      1280,
      720,
      NULL,
      NULL,
      instance,
      appState
  );

  if (window == NULL)
  {
    return 0;
  }

  IDirectSoundBuffer* soundBuffer = {}; 
  Audio::Configuration audioCfg = {};
  audioCfg.samplesPerSecond = 48000;
  audioCfg.frequency = 261;
  audioCfg.volume = 4000;
  audioCfg.wavePeriod = audioCfg.samplesPerSecond / audioCfg.frequency;
  audioCfg.bytesPerSample = sizeof(int16)*2;
  audioCfg.bufferSize = audioCfg.samplesPerSecond * audioCfg.bytesPerSample;
  audioCfg.runningSampleIndex = 0;

  Audio::InitDirectSound(&soundBuffer, window, audioCfg);
  Audio::FillBuffer(soundBuffer, &audioCfg, 0, audioCfg.bufferSize);
  soundBuffer->Play(0, 0, DSBPLAY_LOOPING);

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

  int xOffset = 0;
  int yOffset = 0;
  while (appState->app.isRunning)
  {
    MSG message = {};
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // Only attempt to read controller information if XInput is loaded
    if (XInputGetState) 
    {
      for(int controllerIndex = 0; 
          controllerIndex < XUSER_MAX_COUNT; 
          controllerIndex++)
      {
        XINPUT_STATE controllerState;
        if (XInputGetState(controllerIndex, &controllerState) == ERROR_SUCCESS)
        {
          XINPUT_GAMEPAD* gamepad = &controllerState.Gamepad;

          bool dpadUp = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP;
          bool dpadDown = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
          bool dpadLeft = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
          bool dpadRight = gamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
          bool faceBottom = gamepad->wButtons & XINPUT_GAMEPAD_A;
          bool faceRight = gamepad->wButtons & XINPUT_GAMEPAD_B;
          bool faceLeft = gamepad->wButtons & XINPUT_GAMEPAD_X;
          bool faceTop = gamepad->wButtons & XINPUT_GAMEPAD_Y;
          bool shoulderLeft = gamepad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
          bool shoulderRight = gamepad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
          bool thumbstickLeft = gamepad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
          bool thumbstickRight = gamepad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
          bool start = gamepad->wButtons & XINPUT_GAMEPAD_START;
          bool select = gamepad->wButtons & XINPUT_GAMEPAD_BACK;

          int8 triggerLeft = gamepad->bLeftTrigger;
          int8 triggerRight = gamepad->bRightTrigger;

          int16 xAxisLeft = gamepad->sThumbLX;
          int16 yAxisLeft = gamepad->sThumbLY;
          
          int16 xAxisRight = gamepad->sThumbRX;
          int16 yAxisRight = gamepad->sThumbRY;

          int16 deadzone = 2000;
          if (abs(xAxisLeft) > deadzone)
          {          
            xOffset -= (xAxisLeft >> 12);
          }
          if (abs(yAxisLeft) > deadzone)
          {          
            yOffset += (yAxisLeft >> 12);
          }
        }
        else
        {
          continue;
        }
      }
    }
    
    uint8* keyState = appState->app.keyboard.keyState;
    Input::PoolKeyState(keyState);

    if (Input::IsPressed(keyState[Key::W]) ||
        Input::IsPressed(keyState[Key::UP]))
    {
      yOffset++;
    }
    if (Input::IsPressed(keyState[Key::S]) ||
        Input::IsPressed(keyState[Key::DOWN]))
    {
      yOffset--;
    }
    if (Input::IsPressed(keyState[Key::A]) ||
        Input::IsPressed(keyState[Key::LEFT]))
    {
      xOffset++;
    }
    if (Input::IsPressed(keyState[Key::D]) ||
        Input::IsPressed(keyState[Key::RIGHT]))
    {
      xOffset--;
    }

    App::FrameUpdate(deltaTime);

    Render::RenderGradient(&(appState->app.buffer), xOffset, yOffset);
    
    Audio::TestAudioBuffer(soundBuffer, &audioCfg);

    HDC deviceContext = GetDC(window);
    Win32::BlitBuffer(deviceContext, window, appState);
    ReleaseDC(window, deviceContext);

    QueryPerformanceCounter(&currentCounter);
    int64 counterElapsed = currentCounter.QuadPart - lastCounter.QuadPart;
    deltaTime = (float32)counterElapsed / counterFrequency.QuadPart;

    lastCounter.QuadPart = currentCounter.QuadPart;
  }

  free(appState);
  appState = NULL;
  return 0;
}

/**
 * Callback function required by Windows to allow GUI to recieve and interpret
 * messages from the OS.
 * 
 * @param window The handle to the GUI window that the callback is bound to.
 * @param message A value describing the type of message being received.
 * @param wParam A value determined by Windows and `message`.
 * @param lParam A value determined by Windows and `message`.
 */
LRESULT CALLBACK WindowProc(HWND window, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam)
{
  Win32::AppState* appState = NULL;
  if (message == WM_CREATE)
  {
    CREATESTRUCT* create = (CREATESTRUCT*)lParam;
    appState = (Win32::AppState*)(create->lpCreateParams);
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)appState);
  }
  else
  {
    appState = (Win32::AppState*)(GetWindowLongPtr(window,GWLP_USERDATA));
  }

  switch (message)
  {
    case WM_SIZE:
    {
      return 0;
    }
    case WM_ACTIVATEAPP:
    {
      return 0;
    }
    case WM_CLOSE:
    {
      DestroyWindow(window);
      return 0;
    }
    case WM_DESTROY:
    {
      appState->app.isRunning = false;
      PostQuitMessage(0);
      return 0;
    }
    case WM_PAINT:
    {
      // Paints on create and resize.
      PAINTSTRUCT painter;
      HDC deviceContext = BeginPaint(window, &painter);

      Win32::BlitBuffer(deviceContext, window, appState);

      EndPaint(window, &painter);
      return 0;
    }
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {
      uint8* keyState =  appState->app.keyboard.keyState;

      //NOTE: Unsure if I want to keep map system for input.
      const std::map<size_t, Key> map = appState->app.keyboard.keyMap;

      if (wParam == VK_ESCAPE)
      {
        DestroyWindow(window);
      }

      // Prevents out of bounds access in map
      // Therefore, only registered keys are usable in map
      if (map.find(wParam) == map.end())
      {
        return 0;
      }

      if ((lParam & (1 << 31)) == 0) 
      {
        keyState[map.at(wParam)] |= State::IS_PRESSED;
      }
      else 
      {
        keyState[map.at(wParam)] ^= State::IS_PRESSED; 
      }
      return 0;
    }
  }
  return DefWindowProc(window, message, wParam, lParam);
}
