#include "win32.h"

/**
 * Displays the framebuffer on the window.
 * 
 * @param deviceContext A Windows structure required for displaying graphics.
 * @param window The handle to the GUI window.
 * @param buffer The framebuffer to be displayed.
 * @param bitmapInfo Information Windows requires to display the framebuffer.
 */
void Win32::BlitBuffer(HDC deviceContext, HWND window, Win32::AppState* appState)
{
  RECT clientRect = {};
  GetClientRect(window, &clientRect);
  StretchDIBits(deviceContext,
                0, 0, clientRect.right, clientRect.bottom,
                0, 0, appState->app.buffer.width, appState->app.buffer.height,
                appState->app.buffer.bitmap, 
                &(appState->bitmapInfo),
                DIB_RGB_COLORS, SRCCOPY);
}

/**
 * Changes size allocated in memory for the framebuffer bitmap when the window resizes.
 * 
 * @param appState Struct containing framebuffer and BITMAPINFO
 * @param width The new width of the window.
 * @param height The new height of the window.
 */

void Win32::OnResize(Win32::AppState* appState, int16 width, int16 height)
{
  if (appState->app.buffer.bitmap)
  {
    VirtualFree(appState->app.buffer.bitmap, 0, MEM_RELEASE);
  }

  appState->app.buffer.width = width;
  appState->app.buffer.height = height;
  appState->bitmapInfo.bmiHeader.biWidth = width;
  appState->bitmapInfo.bmiHeader.biHeight = -height;

  const int8 bytesPerPixel = 4;
  int32 bitmapSize = (width * height) * bytesPerPixel;
  
  appState->app.buffer.bitmap = VirtualAlloc(0, bitmapSize, MEM_COMMIT, PAGE_READWRITE);
}

/**
 * Prepares Windows' XInput library and prepares function pointers 
 * to XInput API. Allows for dynamic function loading in case end-user does
 * not have the required XInput dll installed.
 * 
 * @param XInputGetState Function pointer to load XInput function into.
 * Used to get the current state of a controller.
 * @param XInputSetState Function pointer to load XInput function into.
 * Used to set state of a controller for vibration.
 */
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