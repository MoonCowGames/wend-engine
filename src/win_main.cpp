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

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstdbool>
#include <xinput.h>

#include "framebuffer/framebuffer.h"
#include "application/application.h"
#include "input/input.h"
#include "audio/audio.h"

namespace Win32
{
  void BlitBuffer(HDC deviceContext, 
                  HWND window,
                  Render::Framebuffer* buffer, 
                  BITMAPINFO* bitmapInfo);

  /**
   * Displays the framebuffer on the window.
   * 
   * @param deviceContext A Windows structure required for displaying graphics.
   * @param window The handle to the GUI window.
   * @param buffer The framebuffer to be displayed.
   * @param bitmapInfo Information Windows requires to display the framebuffer.
   */
  void BlitBuffer(HDC deviceContext, 
                  HWND window,
                  Render::Framebuffer* buffer, 
                  BITMAPINFO* bitmapInfo)
  {
    RECT clientRect = {};
    GetClientRect(window, &clientRect);
    StretchDIBits(deviceContext,
                  0, 0, clientRect.right, clientRect.bottom,
                  0, 0, buffer->width, buffer->height,
                  buffer->bitmap, bitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
  }
}

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

  App::Application* app = App::InitApplication(width, height);
  
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
      app
  );

  if (window == NULL)
  {
    return 0;
  }

  IDirectSoundBuffer* soundBuffer = {}; 
  Audio::Configuration audioCfg = {};
  audioCfg.samplesPerSecond = 44100;
  audioCfg.frequency = 261;
  audioCfg.volume = 4000;
  audioCfg.wavePeriod = audioCfg.samplesPerSecond / audioCfg.frequency;
  audioCfg.bytesPerSample = sizeof(int16_t)*2;
  audioCfg.bufferSize = audioCfg.samplesPerSecond * audioCfg.bytesPerSample;
  audioCfg.runningSampleIndex = 0;

  Audio::InitDirectSound(&soundBuffer, window, audioCfg);
  Audio::FillBuffer(soundBuffer, &audioCfg, 0, audioCfg.bufferSize);
  soundBuffer->Play(0, 0, DSBPLAY_LOOPING);

  fn_XInputGetState* XInputGetState = nullptr;
  fn_XInputSetState* XInputSetState = nullptr;
  Input::InitXInput(&XInputGetState, &XInputSetState);
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
  while (app->isRunning)
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

          int8_t triggerLeft = gamepad->bLeftTrigger;
          int8_t triggerRight = gamepad->bRightTrigger;

          int16_t xAxisLeft = gamepad->sThumbLX;
          int16_t yAxisLeft = gamepad->sThumbLY;
          
          int16_t xAxisRight = gamepad->sThumbRX;
          int16_t yAxisRight = gamepad->sThumbRY;

          int16_t deadzone = 2000;
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
    
    uint8_t* keyState = app->keyboard.keyState;
    Input::PoolKeyState(keyState);

    if (Input::CheckKeyIsPressed(keyState[Key::W]) ||
        Input::CheckKeyIsPressed(keyState[Key::UP]))
    {
      yOffset++;
    }
    if (Input::CheckKeyIsPressed(keyState[Key::S]) ||
        Input::CheckKeyIsPressed(keyState[Key::DOWN]))
    {
      yOffset--;
    }
    if (Input::CheckKeyIsPressed(keyState[Key::A]) ||
        Input::CheckKeyIsPressed(keyState[Key::LEFT]))
    {
      xOffset++;
    }
    if (Input::CheckKeyIsPressed(keyState[Key::D]) ||
        Input::CheckKeyIsPressed(keyState[Key::RIGHT]))
    {
      xOffset--;
    }

    App::FrameUpdate(deltaTime);

    Render::RenderGradient(&(app->buffer), xOffset, yOffset);
    
    Audio::TestAudioBuffer(soundBuffer, &audioCfg);

    HDC deviceContext = GetDC(window);
    Win32::BlitBuffer(deviceContext, window, 
              &(app->buffer), &(app->bitmapInfo));
    ReleaseDC(window, deviceContext);

    QueryPerformanceCounter(&currentCounter);
    int64_t counterElapsed = currentCounter.QuadPart - lastCounter.QuadPart;
    deltaTime = (float32)counterElapsed / counterFrequency.QuadPart;

    lastCounter.QuadPart = currentCounter.QuadPart;
  }

  free(app);
  app = NULL;
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
  App::Application* appState = NULL;
  if (message == WM_CREATE)
  {
    CREATESTRUCT* create = (CREATESTRUCT*)lParam;
    appState = (App::Application*)(create->lpCreateParams);
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)appState);
  }
  else
  {
    appState = (App::Application*)(GetWindowLongPtr(window,GWLP_USERDATA));
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
      if (MessageBoxA(window, 
                      "Are you sure you want to quit? Unsaved progress will be lost.", 
                      "Wend", 
                      MB_OKCANCEL) == IDOK)
      {
        DestroyWindow(window);
      }
      return 0;
    }
    case WM_DESTROY:
    {
      appState->isRunning = false;
      PostQuitMessage(0);
      return 0;
    }
    case WM_PAINT:
    {
      // Paints on create and resize.
      PAINTSTRUCT painter;
      HDC deviceContext = BeginPaint(window, &painter);

      Win32::BlitBuffer(deviceContext, window, 
                &(appState->buffer), &(appState->bitmapInfo));

      EndPaint(window, &painter);
      return 0;
    }
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {
      uint8_t* keyState =  appState->keyboard.keyState;
      const std::map<size_t, Key> map = appState->keyboard.keyMap;

      if (wParam == VK_ESCAPE)
      {
        if (MessageBoxA(window, 
                        "Are you sure you want to quit? Unsaved progress will be lost.", 
                        "Wend", 
                        MB_OKCANCEL) == IDOK)
        {
          DestroyWindow(window);
        }
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
