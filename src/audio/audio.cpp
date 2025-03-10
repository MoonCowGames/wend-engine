/*
+------------------------------------------------------------------------------+
|File: audio.cpp                                                               |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "audio.h"

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID guiDevice, LPDIRECTSOUND* directSound, LPUNKNOWN outer);
typedef DIRECT_SOUND_CREATE(fn_DirectSoundCreate);

namespace Audio
{

  /**
   * Prepares Windows' DirectSound library and prepares sound buffers.
   * 
   * @param soundBuffer The secondary audio buffer that can be written to.
   * @param window The handle to the GUI window to bind the audio service to.
   * @param config A struct containing configuration information about the 
   * audio service, such as frequency and sample rate.
   */
  void InitDirectSound(IDirectSoundBuffer** soundBuffer, 
                       HWND window, 
                       Configuration config)
  {
    // Get library
    HMODULE directSoundLibrary = LoadLibraryA("dsound.dll");
    if (!directSoundLibrary)
    {
      return;
    }

    // Link function call to library
    fn_DirectSoundCreate *DirectSoundCreate = (fn_DirectSoundCreate *)GetProcAddress(directSoundLibrary, "DirectSoundCreate");
    if (!DirectSoundCreate)
    {
      return;
    }

    // Create DirectSound object
    LPDIRECTSOUND directSoundObj;
    if (DirectSoundCreate(0, &directSoundObj, NULL) < 0)
    {
      return;
    }

    // Bind to window
    if (directSoundObj->SetCooperativeLevel(window, DSSCL_PRIORITY) < 0)
    {
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
      return;
    }
    if (primaryBuffer->SetFormat(&waveFormat) < 0)
    {
      return;
    }

    // Create secondary buffer
    DSBUFFERDESC secondaryBufferDesc;
    ZeroMemory(&secondaryBufferDesc, sizeof(secondaryBufferDesc));
    secondaryBufferDesc.dwSize = sizeof(secondaryBufferDesc);
    secondaryBufferDesc.dwBufferBytes = config.bufferSize;
    secondaryBufferDesc.lpwfxFormat = &waveFormat;

    if (directSoundObj->CreateSoundBuffer(
        &secondaryBufferDesc, soundBuffer, NULL) < 0)
    {
      return;
    }
  }

  // NOTE: This will change or be removed. No docstring needed.
  void TestAudioBuffer(IDirectSoundBuffer* soundBuffer, 
                       Configuration* config)
  {
    //int32 halfPeriod = wavePeriod >> 1;
    int32 bytesPerSample = sizeof(int16)*2;

    DWORD playCursor;
    DWORD writeCursor;

    if (soundBuffer->GetCurrentPosition(&playCursor, &writeCursor) < 0)
    {
      return;
    }

    // Keeps range within bufferSize values
    DWORD lockCursor = ((config->runningSampleIndex) * bytesPerSample) % config->bufferSize;
    DWORD bytesToWrite = 0;

    if (lockCursor == playCursor)
    {
        bytesToWrite = 0;
    }
    else if (lockCursor > playCursor)
    {
      // Gets space marked ====
      // ||==============[PC]------------[LC]================||
      bytesToWrite = (config->bufferSize - lockCursor);
      bytesToWrite += playCursor; 
    }
    else
    {
      // Gets space marked ====
      // ||--------------[LC]============[PC]----------------||
      bytesToWrite = playCursor - lockCursor;
    }

    FillBuffer(soundBuffer, config, lockCursor, bytesToWrite);
    
  }

  // NOTE: This will change or be removed. No docstring needed.
  int16 SineWave(float32 time, int32 volume)
  {
    return (int16)(sinf(time)*(float32)volume);
  }

  /**
   * Fills sound buffer with a given audio sample.
   * TODO: Change to accepting an audio sample as parameter. Currently just uses a sine wave.
   * 
   * @param soundBuffer The secondary audio buffer that can be written to.
   * @param config A struct containing configuration information about the 
   * audio service, such as frequency and sample rate.
   * @param lockCursor Determines the point of the buffer to lock and prepare to write to.
   * @param bytesToWrite The number of bytes expected to write to.
   */
  void FillBuffer(IDirectSoundBuffer* soundBuffer, 
                  Configuration* config, 
                  DWORD lockCursor, 
                  DWORD bytesToWrite)
  {
    void* region1;
    DWORD region1Size;
    void* region2;
    DWORD region2Size;

    if (soundBuffer->Lock(lockCursor, bytesToWrite,
                          &region1, &region1Size,
                          &region2, &region2Size, 0) < 0)
    {
      return;
    }

    int16* sample = (int16 *)region1;
    DWORD region1SampleCount = region1Size/config->bytesPerSample;
    for (uint32 index = 0; index < region1SampleCount; index++)
    {
      float32 time = 2.0f * PI32 * ((float32)(config->runningSampleIndex) / (float32)config->wavePeriod); 
      int16 sampleValue = sinf(time) * 4000;
      // left
      *sample = sampleValue;
      sample++;
      // right
      *sample = sampleValue;
      sample++;
      (config->runningSampleIndex)++;
    }
    
    sample = (int16 *)region2;
    DWORD region2SampleCount = region2Size/config->bytesPerSample;
    for (uint32 index = 0; index < region2SampleCount; index++)
    {
      float32 time = 2.0f * PI32 * 
          ((float32)(config->runningSampleIndex) / (float32)config->wavePeriod); 
      int16 sampleValue = sinf(time) * 4000;
      // left
      *sample = sampleValue;
      sample++;
      // right
      *sample = sampleValue;
      sample++;
      (config->runningSampleIndex)++;
    }

    soundBuffer->Unlock(region1, region1Size, region2, region2Size);
  }
}