#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstdbool>

#include "framebuffer/framebuffer.h"

namespace win32
{
  // TODO: Extract to module
  // StateInfo (consider renaming to Application) defines application state.
  // It is inteded for crossplatform usage, and will need to be extracted into its own header. 
  // We can implement Win32 specific struct using preprocessor directives.
  struct StateInfo
  {
    bool isRunning;
    BITMAPINFO bitmapInfo;
    render::Framebuffer buffer;
  };

  void RenderGradient(render::Framebuffer* buffer, int xOffset, int yOffset);
  void BlitBitmap(HDC deviceContext, 
                  RECT* clientRect, 
                  render::Framebuffer* buffer, 
                  BITMAPINFO* bitmapInfo);
  StateInfo* GetAppState(HWND window);

  void BlitBitmap(HDC deviceContext, 
                  RECT* clientRect, 
                  render::Framebuffer* buffer, 
                  BITMAPINFO* bitmapInfo)
  {
    StretchDIBits(deviceContext,
                  0, 0, buffer->width, buffer->height,
                  0, 0, clientRect->right, clientRect->bottom,
                  buffer->bitmap, bitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
  }

  void RenderGradient(render::Framebuffer* buffer, int xOffset, int yOffset)
  {
    int pitch = buffer->width*4;
    uint8_t* row = (uint8_t*)buffer->bitmap;
    for(int y = 0; y < buffer->height; ++y)
    {
      uint32_t* pixel = (uint32_t*)row;
      for(int x = 0; x < buffer->width; ++x)
      {
        *pixel = (uint8_t)(x+xOffset) << 16 | 
                (uint8_t)(y+yOffset) << 8 | 
                255;
        ++pixel;
      }
      row += pitch;
    }
  }

  StateInfo* GetAppState(HWND window)
  {
    StateInfo* state = (StateInfo*)(GetWindowLongPtr(window, GWLP_USERDATA));
    return state;
  }
}

LRESULT CALLBACK WindowProc(HWND window, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam);

int WINAPI WinMain(HINSTANCE instance, 
                  HINSTANCE prevInstance, 
                  PSTR cmdLine,
                  int cmdShow)
{
  win32::StateInfo* appState = 
      (win32::StateInfo*)malloc(sizeof(win32::StateInfo));
  appState->isRunning = true;
  appState->bitmapInfo.bmiHeader.biSize = sizeof(appState->bitmapInfo.bmiHeader);
  appState->bitmapInfo.bmiHeader.biPlanes = 1;
  appState->bitmapInfo.bmiHeader.biBitCount = 32;
  appState->bitmapInfo.bmiHeader.biCompression = BI_RGB;

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
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      instance,
      appState
  );

  if (window == NULL)
  {
    return 0;
  }

  ShowWindow(window, cmdShow);
  UpdateWindow(window);

  int xOffset = 0;
  int yOffset = 0;
  while (appState->isRunning)
  {
    MSG message = {};
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
    
    xOffset++;
    yOffset++;
    win32::RenderGradient(&(appState->buffer), xOffset, yOffset);
    
    HDC deviceContext = GetDC(window);
    RECT clientRect = {};
    GetClientRect(window, &clientRect);
    win32::BlitBitmap(deviceContext, &clientRect, 
              &(appState->buffer), &(appState->bitmapInfo));
    ReleaseDC(window, deviceContext);
  }

  free(appState);
  appState = NULL;
  return 0;
}

LRESULT CALLBACK WindowProc(HWND window, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam)
{
  win32::StateInfo* state = NULL;
  if (message == WM_CREATE)
  {
    CREATESTRUCT* create = (CREATESTRUCT*)lParam;
    state = (win32::StateInfo*)(create->lpCreateParams);
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)state);
  }
  else
  {
    state = win32::GetAppState(window);
  }

  switch (message)
  {
    case WM_SIZE:
    {

      RECT clientRect;
      GetClientRect(window, &clientRect);
      state->bitmapInfo.bmiHeader.biWidth = clientRect.right;
      state->bitmapInfo.bmiHeader.biHeight = -clientRect.bottom;
      render::ResizeFramebuffer(&(state->buffer), 
                              clientRect.right, clientRect.bottom);
      return 0;
    }
    case WM_ACTIVATEAPP:
    {
      return 0;
    }
    case WM_CLOSE:
    {
      if (MessageBoxA(window, "Are you sure you want to quit? Unsaved progress will be lost.", "Wend", MB_OKCANCEL) == IDOK)
      {
        DestroyWindow(window);
      }
      return 0;
    }
    case WM_DESTROY:
    {
      state->isRunning = false;
      PostQuitMessage(0);
      return 0;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT painter;
      HDC deviceContext = BeginPaint(window, &painter);

      RECT clientRect = {};
      GetClientRect(window, &clientRect);
      win32::BlitBitmap(deviceContext, &clientRect, 
                &(state->buffer), &(state->bitmapInfo));
      EndPaint(window, &painter);
      return 0;
    }
  }
  return DefWindowProc(window, message, wParam, lParam);
}
