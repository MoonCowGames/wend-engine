/*
+------------------------------------------------------------------------------+
|File: wend_winapi.h                                                           |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_WINAPI_H__
#define __WEND_WINAPI_H__

#include <windows.h>
#include <windowsx.h>
#include <xinput.h>

#include "win_input.h"
#include "../../framebuffer/framebuffer.h"
#include "../../application/application.h"
#include "../../input/input.h"
#include "../../sound/sound.h"
#include "../../misc/includes.h"

// Handling dynamic loading of DLLs
#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef XINPUT_GET_STATE(fn_XInputGetState);

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef XINPUT_SET_STATE(fn_XInputSetState);

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPGUID guiDevice, LPDIRECTSOUND* directSound, LPUNKNOWN outer);
typedef DIRECT_SOUND_CREATE(fn_DirectSoundCreate);

#define KILOBYTE(x) (uint64)(x*1024)
#define MEGABYTE(x) (uint64)(KILOBYTE(x)*1024)
#define GIGABYTE(x) (uint64)(MEGABYTE(x)*1024)

namespace Win32
{
      /// @struct AppState Provides Windows with a handle to the application and associated data.
  struct AppState
  {
    App::Application app = {0};
    BITMAPINFO bitmapInfo = {0};
  };

  struct MemoryArena
  {
    void* permanent;
    uint64 permanentSize;
    
    void* transient;
    uint64 transientSize;
  };

      /// @brief Displays the framebuffer on the window.
      /// @param deviceContext A Windows structure required for displaying graphics.
      /// @param window The handle to the GUI window.
      /// @param buffer The framebuffer to be displayed.
      /// @param bitmapInfo Information Windows requires to display the framebuffer.
  void BlitBuffer(
    HDC deviceContext, 
    HWND window, 
    AppState* appState);

      /// Prepares Windows' XInput library and prepares function pointers .
      /// to XInput API. Allows for dynamic function loading in case end-user does.
      /// not have the required XInput dll installed.
      /// 
      /// @param XInputGetState Function pointer to load XInput function into. Used to get the current state of a controller.
      /// @param XInputSetState Function pointer to load XInput function into. Used to set state of a controller for vibration.
  void InitXInput(
    fn_XInputGetState** XInputGetState, 
    fn_XInputSetState** XInputSetState);

      /// @brief Checks and updates state of each controller.
      /// @param XInputGetState Function pointer to load XInput function into. Used to get the current state of a controller.
      /// @param appState Provides Windows with a handle to the application and associated data.
  void PoolGamepadInput(
    fn_XInputGetState* XInputGetState, 
    AppState* appState);

      /// @brief Changes size allocated in memory for the framebuffer bitmap when the window resizes.
      /// @param appState Struct containing app data.
      /// @param width The new width of the window.
      /// @param height The new height of the window.
  void OnResize(
    AppState* appState, 
    int16 width,
    int16 height);

      /// @brief Prepares Windows' DirectSound library and prepares sound buffers.
      /// @param directSoundBuffer The secondary audio buffer that can be written to.
      /// @param window The handle to the GUI window to bind the audio service to.
      /// @param soundCfg Platform independant sound config format.
  void InitDirectSoundBuffer(
    HWND window, 
    IDirectSoundBuffer** directSoundBuffer, 
    Sound::Configuration* soundCfg);
  
      /// @brief Fills sound buffer with a given audio sample.
      /// TODO: Change to accepting an audio sample as parameter. Currently just uses a sine wave.
      /// 
      /// @param directSoundBuffer Windows sound buffer.
      /// @param sourceSoundBuffer Platform independant sound buffer.
      /// @param soundCfg Platform independant sound config format.
      /// @param lockCursor Determines the point of the buffer to lock and prepare to write to.
      /// @param bytesToWrite The number of bytes expected to write to.
  void FillDirectSoundBuffer(
    IDirectSoundBuffer* directSoundBuffer, 
    Sound::Buffer* sourceSoundBuffer, 
    Sound::Configuration* soundCfg, 
    DWORD lockCursor, 
    DWORD bytesToWrite);

      /// @brief Zeroes out sound buffer.
      /// @param directSoundBuffer Windows sound buffer.
      /// @param soundCfg Platform independant sound config format.
      /// @param lockCursor Determines the point of the buffer to lock and prepare to write to.
      /// @param bytesToWrite The number of bytes expected to write to.
  void ClearDirectSoundBuffer(
    IDirectSoundBuffer* directSoundBuffer, 
    Sound::Configuration* soundCfg, 
    DWORD lockCursor, 
    DWORD bytesToWrite);
  
      /// @brief Retrieves state of sound buffer.
      /// @param directSoundBuffer Windows sound buffer.
      /// @param sourceSoundBuffer Platform independant sound buffer.
      /// @param soundCfg Platform independant sound config format.
      /// @param lockCursor Determines the point of the buffer to lock and prepare to write to.
      /// @param bytesToWrite The number of bytes expected to write to.
  void GetDirectSoundState(
    IDirectSoundBuffer* directSoundBuffer, 
    Sound::Buffer* sourceSoundBuffer, 
    Sound::Configuration* soundCfg, 
    DWORD* lockCursor, 
    DWORD* bytesToWrite);

      /// @brief Init and register Window Class.
      /// @param windowClass Handle to window class.
      /// @param instance Window instance.
      /// @param className Identifier for window class.
      /// @return false on fail, true on success.
  bool RegisterWindowClass(
    WNDCLASSA* windowClass, 
    HINSTANCE instance, 
    const char* className);

      /// @brief Init and create Window.
      /// @param window The handle to the GUI window.
      /// @param appState Struct containing app data.
      /// @param instance Window instance.
      /// @param className Identifier for window class.
      /// @return false on fail, true on success.
  bool CreateWin32Window(
    HWND* window,
    AppState* appstate,
    HINSTANCE instance,
    const char* className);

      /// @brief Init BITMAPINFOHEADER for Windows
      /// @param bmiHeader Header to init
  void InitBitmapHeader(BITMAPINFOHEADER* bmiHeader);

  void CreateMemoryArena(MemoryArena *memArena);

      /// @brief Callback function required by Windows to allow GUI to recieve and interpret messages from the OS.
      /// @param window The handle to the GUI window.
      /// @param message A value describing the type of message being received.
      /// @param wParam A value determined by Windows and `message`.
      /// @param lParam A value determined by Windows and `message`.
  LRESULT CALLBACK WindowProc(
    HWND window, 
    UINT message, 
    WPARAM wParam, 
    LPARAM lParam);
}

#endif //__WEND_WINAPI_H__