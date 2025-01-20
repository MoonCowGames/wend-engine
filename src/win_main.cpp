#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstdbool>

#include "framebuffer/framebuffer.h"
#include "application/application.h"
#include "input/input.h"

namespace win32
{
  void BlitBuffer(HDC deviceContext, 
                  HWND window,
                  Render::Framebuffer* buffer, 
                  BITMAPINFO* bitmapInfo);
  App::Application* GetAppState(HWND window);

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

  App::Application* GetAppState(HWND window)
  {
    return (App::Application*)(GetWindowLongPtr(window,GWLP_USERDATA));
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

  ShowWindow(window, cmdShow);
  UpdateWindow(window);

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
    
    uint8_t* keyState = app->keyboard.keyState;

    Render::RenderGradient(&(app->buffer), xOffset, yOffset);
    
    HDC deviceContext = GetDC(window);
    
    win32::BlitBuffer(deviceContext, window, 
              &(app->buffer), &(app->bitmapInfo));
    ReleaseDC(window, deviceContext);
  }

  free(app);
  app = NULL;
  return 0;
}

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
    appState = win32::GetAppState(window);
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
      if (MessageBoxA(window, "Are you sure you want to quit? Unsaved progress will be lost.", "Wend", MB_OKCANCEL) == IDOK)
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

      win32::BlitBuffer(deviceContext, window, 
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
        if (MessageBoxA(window, "Are you sure you want to quit? Unsaved progress will be lost.", "Wend", MB_OKCANCEL) == IDOK)
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
