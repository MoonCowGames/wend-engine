#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstdbool>

struct Buffer
{
  int width;
  int height;
  void* bitmap;
};

struct StateInfo
{
  bool isRunning;
  BITMAPINFO bitmapInfo;
  Buffer buffer;
};

LRESULT CALLBACK WindowProc(HWND window, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam);

void ResizeBuffer(Buffer* buffer, BITMAPINFO* bitmapInfo, int width, int height);
void RenderGradient(Buffer* buffer, int xOffset, int yOffset);
void BlitBitmap(HDC deviceContext, 
                RECT* windowRect, 
                Buffer* buffer, 
                BITMAPINFO* bitmapInfo);
StateInfo* GetAppState(HWND window);

int WINAPI WinMain(HINSTANCE instance, 
                   HINSTANCE prevInstance, 
                   PSTR cmdLine,
                   int cmdShow)
{
  StateInfo* appState = (StateInfo*)malloc(sizeof(StateInfo));
  appState->isRunning = true;
  appState->bitmapInfo.bmiHeader.biSize = sizeof(appState->bitmapInfo.bmiHeader);
  appState->bitmapInfo.bmiHeader.biWidth = 0;
  appState->bitmapInfo.bmiHeader.biHeight = 0;
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
    RenderGradient(&(appState->buffer), xOffset, yOffset);
    
    HDC deviceContext = GetDC(window);
    RECT clientRect = {};
    GetClientRect(window, &clientRect);
    BlitBitmap(deviceContext, &clientRect, 
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
  StateInfo* state = NULL;
  if (message == WM_CREATE)
  {
    CREATESTRUCT* create = (CREATESTRUCT*)lParam;
    state = (StateInfo*)(create->lpCreateParams);
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)state);
  }
  else
  {
    state = GetAppState(window);
  }

  switch (message)
  {
    case WM_SIZE:
    {
      RECT clientRect;
      GetClientRect(window, &clientRect);
      int width = clientRect.right;
      int height = clientRect.bottom;
      ResizeBuffer(&(state->buffer), &(state->bitmapInfo), width, height);
      return 0;
    }
    case WM_ACTIVATEAPP:
    {
      return 0;
    }
    case WM_CLOSE:
    {
      //if (MessageBox(window, "Are you sure you want to quit? Unsaved progress will be lost.", "Wend", MB_OKCANCEL) == IDOK)
      //{
      DestroyWindow(window);
      //}
      return 0;
    }
    case WM_DESTROY:
    {
      state->isRunning = false;
      return 0;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT painter;
      HDC deviceContext = BeginPaint(window, &painter);

      RECT clientRect = {};
      GetClientRect(window, &clientRect);
      BlitBitmap(deviceContext, &clientRect, 
                 &(state->buffer), &(state->bitmapInfo));
      EndPaint(window, &painter);
      return 0;
    }
  }
  return DefWindowProc(window, message, wParam, lParam);
}

void ResizeBuffer(Buffer* buffer, BITMAPINFO* bitmapInfo, int width, int height)
{
  if (buffer->bitmap)
  {
    VirtualFree(buffer->bitmap, 0, MEM_RELEASE);
  }

  buffer->width = width;
  buffer->height = height;
  
  bitmapInfo->bmiHeader.biWidth = width;
  bitmapInfo->bmiHeader.biHeight = -height;

  const int bytesPerPixel = 4;
  int bitmapSize = (width * height) * bytesPerPixel;
  
  buffer->bitmap = VirtualAlloc(0, bitmapSize, MEM_COMMIT, PAGE_READWRITE);
}

void BlitBitmap(HDC deviceContext, 
                RECT* windowRect, 
                Buffer* buffer, 
                BITMAPINFO* bitmapInfo)
{
  int windowWidth = windowRect->right - windowRect->left;
  int windowHeight = windowRect->bottom - windowRect->top;
  StretchDIBits(deviceContext,
                0, 0, buffer->width, buffer->height,
                0, 0, windowWidth, windowHeight,
                buffer->bitmap, bitmapInfo,
                DIB_RGB_COLORS, SRCCOPY);
}

void RenderGradient(Buffer* buffer, int xOffset, int yOffset)
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