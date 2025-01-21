#include "audio.h"

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID guiDevice, LPDIRECTSOUND* directSound, LPUNKNOWN outer);
typedef DIRECT_SOUND_CREATE(fn_DirectSoundCreate);

namespace Audio
{

  void InitDirectSound(IDirectSoundBuffer** soundBuffer, 
                       HWND window, 
                       uint32_t samplesPerSec)
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
    waveFormat.nSamplesPerSec = samplesPerSec;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign = 
        (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
    waveFormat.nAvgBytesPerSec = 
        waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;
    int32_t bufferSize = waveFormat.nAvgBytesPerSec;

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
    secondaryBufferDesc.dwBufferBytes = bufferSize;
    secondaryBufferDesc.lpwfxFormat = &waveFormat;

    if (directSoundObj->CreateSoundBuffer(
        &secondaryBufferDesc, soundBuffer, NULL) < 0)
    {
      return;
    }
  }

}