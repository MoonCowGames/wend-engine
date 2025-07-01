#include "win32.h"

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