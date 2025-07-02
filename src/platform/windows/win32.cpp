/*
+------------------------------------------------------------------------------+
|File: win32.h                                                                 |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "win32.h"

void Win32::BlitBuffer(HDC deviceContext, HWND window, Win32::AppState* appState)
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

void Win32::InitXInput(fn_XInputGetState** XInputGetState, fn_XInputSetState** XInputSetState)
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

void Win32::OnResize(Win32::AppState* appState, int16 width, int16 height)
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

void Win32::InitDirectSoundBuffer(
  IDirectSoundBuffer** directSoundBuffer, 
  HWND window, 
  Sound::Configuration config)
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
  waveFormat.nSamplesPerSec = config.samplesPerSecond;
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
  secondaryBufferDesc.dwBufferBytes = config.bufferSize;
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
  Sound::Configuration* config, 
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
  DWORD region1SampleCount = region1Size/config->bytesPerSample;
  for (uint32 index = 0; index < region1SampleCount; index++)
  {
    // left
    *destSample++ = 0;
    // right
    *destSample++ = 0;
  }
  
  destSample = (int16 *)region2;
  DWORD region2SampleCount = region2Size/config->bytesPerSample;
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
  Sound::Configuration* config, 
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
  DWORD region1SampleCount = region1Size/config->bytesPerSample;
  for (
    uint32 index = 0;
    index < region1SampleCount && index < sourceSoundBuffer->sampleCount; 
    index++
  )
  {
    // left
    *destSample++ = *srcSample++;
    // right
    *destSample++ = *srcSample++;
    (config->runningSampleIndex)++;
  }
  
  destSample = (int16 *)region2;
  DWORD region2SampleCount = region2Size/config->bytesPerSample;
  for (
    uint32 index = 0; 
    index < region2SampleCount && index < sourceSoundBuffer->sampleCount; 
    index++
  )
  {
    
    // left
    *destSample++ = *srcSample++;
    // right
    *destSample++ = *srcSample++;
    (config->runningSampleIndex)++;
  }
  
  directSoundBuffer->Unlock(region1, region1Size, region2, region2Size);
}
