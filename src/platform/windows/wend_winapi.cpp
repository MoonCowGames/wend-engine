/*
+------------------------------------------------------------------------------+
|File: wend_winapi.h                                                           |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "wend_winapi.h"

// Window Services

void Win32::BlitBuffer(
  HDC deviceContext, 
  HWND window, 
  AppState* appState)
{
  RECT clientRect = {};
  GetClientRect(window, &clientRect);
  StretchDIBits(deviceContext,
                0, 0, clientRect.right, clientRect.bottom,
                0, 0, appState->app.frameBuffer.width, appState->app.frameBuffer.height,
                appState->app.frameBuffer.bitmap, 
                &(appState->bitmapInfo),
                DIB_RGB_COLORS, SRCCOPY);
}

void Win32::OnResize(
  AppState* appState, 
  int16 width, 
  int16 height)
{
  if (appState->app.frameBuffer.bitmap)
  {
    VirtualFree(appState->app.frameBuffer.bitmap, 0, MEM_RELEASE);
  }

  appState->app.frameBuffer.width = width;
  appState->app.frameBuffer.height = height;
  appState->bitmapInfo.bmiHeader.biWidth = width;
  appState->bitmapInfo.bmiHeader.biHeight = -height;

  const int8 bytesPerPixel = 4;
  int32 bitmapSize = (width * height) * bytesPerPixel;
  
  appState->app.frameBuffer.bitmap = VirtualAlloc(
    0, 
    bitmapSize, 
    MEM_RESERVE | MEM_COMMIT, 
    PAGE_READWRITE
  );
}

// Gamepad Services

void Win32::InitXInput(
  fn_XInputGetState** XInputGetState, 
  fn_XInputSetState** XInputSetState)
{
  // Get library
  HMODULE xInputLibrary = LoadLibraryA("xinput1_3.dll");
  if (!xInputLibrary)
  {
    return;
  }
  
  // Link function call to library
  *XInputGetState = (fn_XInputGetState *)GetProcAddress(xInputLibrary, "XInputGetState");
  if (!XInputGetState)
  {
    return;
  }
  *XInputSetState = (fn_XInputSetState *)GetProcAddress(xInputLibrary, "XInputSetState");
  if (!XInputSetState)
  {
    return;
  }
}


void Win32::PoolGamepadInput(
  fn_XInputGetState* XInputGetState, 
  AppState* appState)
{
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
        XINPUT_GAMEPAD* systemGamepad = &controllerState.Gamepad;
        Input::Gamepad* appGamepad = &(appState->app.gamepad[controllerIndex]);
        appGamepad->dpadUp = (systemGamepad->wButtons & XINPUT_GAMEPAD_DPAD_UP) > 0;
        appGamepad->dpadDown = (systemGamepad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN) > 0;
        appGamepad->dpadLeft = (systemGamepad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT) > 0;
        appGamepad->dpadRight = (systemGamepad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) > 0;
        appGamepad->faceBottom = (systemGamepad->wButtons & XINPUT_GAMEPAD_A) > 0;
        appGamepad->faceRight = (systemGamepad->wButtons & XINPUT_GAMEPAD_B) > 0;
        appGamepad->faceLeft = (systemGamepad->wButtons & XINPUT_GAMEPAD_X) > 0;
        appGamepad->faceTop = (systemGamepad->wButtons & XINPUT_GAMEPAD_Y) > 0;
        appGamepad->shoulderLeft = (systemGamepad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) > 0;
        appGamepad->shoulderRight = (systemGamepad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) > 0;
        appGamepad->thumbstickLeft = (systemGamepad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB) > 0;
        appGamepad->thumbstickRight = (systemGamepad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) > 0;
        appGamepad->start = (systemGamepad->wButtons & XINPUT_GAMEPAD_START) > 0;
        appGamepad->select = (systemGamepad->wButtons & XINPUT_GAMEPAD_BACK) > 0;

        appGamepad->triggerLeft = systemGamepad->bLeftTrigger;
        appGamepad->triggerRight = systemGamepad->bRightTrigger;

        appGamepad->xAxisLeft = systemGamepad->sThumbLX;
        appGamepad->yAxisLeft = systemGamepad->sThumbLY;
        
        appGamepad->xAxisRight = systemGamepad->sThumbRX;
        appGamepad->yAxisRight = systemGamepad->sThumbRY;
      }
      else
      {
        continue;
      }
    }
  }
}

// Sound Services

void Win32::InitDirectSoundBuffer(
  HWND window, 
  IDirectSoundBuffer** directSoundBuffer, 
  Sound::Configuration* soundCfg)
{
  // Get library
  HMODULE directSoundLibrary = LoadLibraryA("dsound.dll");
  if (!directSoundLibrary)
  {
    // TODO: Log error
    return;
  }

  // Link function call to library
  fn_DirectSoundCreate *DirectSoundCreate = (fn_DirectSoundCreate *)GetProcAddress(directSoundLibrary, "DirectSoundCreate");
  if (!DirectSoundCreate)
  {
    // TODO: Log error
    return;
  }

  // Create DirectSound object
  LPDIRECTSOUND directSoundObj;
  if (DirectSoundCreate(0, &directSoundObj, NULL) < 0)
  {
    // TODO: Log error
    return;
  }

  // Bind to window
  if (directSoundObj->SetCooperativeLevel(window, DSSCL_PRIORITY) < 0)
  {
    // TODO: Log error
    return;
  }

  // Assign buffer format
  WAVEFORMATEX waveFormat;
  ZeroMemory(&waveFormat, sizeof(waveFormat));
  waveFormat.wFormatTag = WAVE_FORMAT_PCM;
  waveFormat.nChannels = 2;
  waveFormat.nSamplesPerSec = soundCfg->samplesPerSecond;
  waveFormat.wBitsPerSample = 16;
  waveFormat.nBlockAlign = 
      (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
  waveFormat.nAvgBytesPerSec = 
      waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
  waveFormat.cbSize = 0;

  // Create primary buffer
  DSBUFFERDESC primaryBufferDesc;
  ZeroMemory(&primaryBufferDesc, sizeof(primaryBufferDesc));
  primaryBufferDesc.dwSize = sizeof(primaryBufferDesc);
  primaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
  IDirectSoundBuffer* primaryBuffer;
  if (directSoundObj->CreateSoundBuffer(
      &primaryBufferDesc, &primaryBuffer, NULL) < 0)
  {
    // TODO: Log error
    return;
  }
  if (primaryBuffer->SetFormat(&waveFormat) < 0)
  {
    // TODO: Log error
    return;
  }

  // Create secondary buffer
  DSBUFFERDESC secondaryBufferDesc;
  ZeroMemory(&secondaryBufferDesc, sizeof(secondaryBufferDesc));
  secondaryBufferDesc.dwSize = sizeof(secondaryBufferDesc);
  secondaryBufferDesc.dwBufferBytes = soundCfg->bufferSize;
  secondaryBufferDesc.lpwfxFormat = &waveFormat;

  if (directSoundObj->CreateSoundBuffer(
      &secondaryBufferDesc, directSoundBuffer, NULL) < 0)
  {
    // TODO: Log error
    return;
  }
}

void Win32::ClearDirectSoundBuffer(
  IDirectSoundBuffer* directSoundBuffer, 
  Sound::Configuration* soundCfg, 
  DWORD lockCursor, 
  DWORD bytesToWrite)
{
  void* region1;
  DWORD region1Size;
  void* region2;
  DWORD region2Size;
  
  if (directSoundBuffer->Lock(lockCursor, bytesToWrite,
        &region1, &region1Size,
        &region2, &region2Size, 0) < 0)
  {
    // TODO: Log error
    return;
  }
  
  int16* destSample = (int16 *)region1;
  DWORD region1SampleCount = region1Size/soundCfg->bytesPerSample;
  for (uint32 index = 0; index < region1SampleCount; index++)
  {
    // left
    *destSample++ = 0;
    // right
    *destSample++ = 0;
  }
  
  destSample = (int16 *)region2;
  DWORD region2SampleCount = region2Size/soundCfg->bytesPerSample;
  for (uint32 index = 0; index < region2SampleCount; index++)
  {
    // left
    *destSample++ = 0;
    // right
    *destSample++ = 0;
  }
  
  directSoundBuffer->Unlock(region1, region1Size, region2, region2Size);
}

void Win32::FillDirectSoundBuffer(
  IDirectSoundBuffer* directSoundBuffer, 
  Sound::Buffer* sourceSoundBuffer,
  Sound::Configuration* soundCfg, 
  DWORD lockCursor, 
  DWORD bytesToWrite)
{
  void* region1;
  DWORD region1Size;
  void* region2;
  DWORD region2Size;
  
  if (directSoundBuffer->Lock(lockCursor, bytesToWrite,
        &region1, &region1Size,
        &region2, &region2Size, 0) < 0)
  {
    // TODO: Log error
    return;
  }
  
  int16* destSample = (int16 *)region1;
  int16* srcSample = sourceSoundBuffer->samples;
  DWORD region1SampleCount = region1Size/soundCfg->bytesPerSample;
  for (
    int32 index = 0;
    index < (int32)region1SampleCount && index < sourceSoundBuffer->sampleCount; 
    index++
  )
  {
    // left
    *destSample++ = *srcSample++;
    // right
    *destSample++ = *srcSample++;
    (soundCfg->runningSampleIndex)++;
  }
  
  destSample = (int16 *)region2;
  DWORD region2SampleCount = region2Size/soundCfg->bytesPerSample;
  for (
    int32 index = 0; 
    index < (int32)region2SampleCount && index < sourceSoundBuffer->sampleCount; 
    index++
  )
  {
    // left
    *destSample++ = *srcSample++;
    // right
    *destSample++ = *srcSample++;
    (soundCfg->runningSampleIndex)++;
  }
  
  directSoundBuffer->Unlock(region1, region1Size, region2, region2Size);
}


void Win32::GetDirectSoundState(
  IDirectSoundBuffer* directSoundBuffer, 
  Sound::Buffer* sourceSoundBuffer,
  Sound::Configuration* soundCfg, 
  DWORD* lockCursor, 
  DWORD* bytesToWrite)
{
  DWORD playCursor = 0;
  DWORD writeCursor = 0;

  if (directSoundBuffer->GetCurrentPosition(&playCursor, &writeCursor) < 0)
  {
    // TODO: Log error
    return;
  }

  // Keeps range within bufferSize values
  *lockCursor = ((soundCfg->runningSampleIndex) * soundCfg->bytesPerSample) % soundCfg->bufferSize;

  if (*lockCursor > playCursor)
  {
    // Gets space marked ====
    // ||==============[PC]------------[LC]================||
    *bytesToWrite = (soundCfg->bufferSize - *lockCursor);
    *bytesToWrite += playCursor; 
  }
  else
  {
    // Gets space marked ====
    // ||--------------[LC]============[PC]----------------||
    *bytesToWrite = playCursor - *lockCursor;
  }

  sourceSoundBuffer->sampleCount = *bytesToWrite / soundCfg->bytesPerSample;
}


LRESULT CALLBACK Win32::WindowProc(
  HWND window, 
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
      if (wParam == VK_ESCAPE)
      {
        DestroyWindow(window);
      }

      // TODO: Change to event system
      if ((lParam & (1 << 31)) == 0) // KeyDown
      {
        appState->app.keyboard.keyState[Win32::TranslateKeyboard(wParam)] |= State::IS_PRESSED;
      }
      else  // KeyUp
      {
        appState->app.keyboard.keyState[Win32::TranslateKeyboard(wParam)] ^= State::IS_PRESSED; 
      }
      return 0;
    }

    case WM_MOUSEMOVE:
    {
      appState->app.mouse.xPos = GET_X_LPARAM(lParam);
      appState->app.mouse.yPos = GET_Y_LPARAM(lParam);
      return 0;
    }

    case WM_LBUTTONDOWN:
    {
      appState->app.mouse.leftButton |= State::IS_PRESSED;
      return 0;
    }
    case WM_LBUTTONUP:
    {
      appState->app.mouse.leftButton ^= State::IS_PRESSED;
      return 0;
    }
    
    case WM_RBUTTONDOWN:
    {
      appState->app.mouse.rightButton |= State::IS_PRESSED;
      return 0;
    }
    case WM_RBUTTONUP:
    {
      appState->app.mouse.rightButton ^= State::IS_PRESSED;
      return 0;
    }

    case WM_MBUTTONDOWN:
    {
      appState->app.mouse.middleButton |= State::IS_PRESSED;
      return 0;
    }
    case WM_MBUTTONUP:
    {
      appState->app.mouse.middleButton ^= State::IS_PRESSED;
      return 0;
    }

    case WM_XBUTTONDOWN:
    {
      int32 xButton = GET_XBUTTON_WPARAM(wParam);
      if (xButton == XBUTTON1)
      {
        appState->app.mouse.thumb1Button |= State::IS_PRESSED;
      }
      else if (xButton == XBUTTON2)
      {
        appState->app.mouse.thumb2Button |= State::IS_PRESSED;
      }
      return 1;
    }
    case WM_XBUTTONUP:
    {
      int32 xButton = GET_XBUTTON_WPARAM(wParam);
      if (xButton == XBUTTON1)
      {
        appState->app.mouse.thumb1Button ^= State::IS_PRESSED;
      }
      else if (xButton == XBUTTON2)
      {
        appState->app.mouse.thumb2Button ^= State::IS_PRESSED;
      }
      return 1;
    }

    case WM_MOUSEWHEEL:
    {
      appState->app.mouse.wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
      return 0;
    }
  }
  return DefWindowProc(window, message, wParam, lParam);
}