#include <windows.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstdbool>

// TODO: Remove global variables
// These are temporary while getting our feet of the ground.
static BITMAPINFO bitmapInfo;
static void* bitmap;
static int bitmapWidth;
static int bitmapHeight;

struct StateInfo
{
  bool isRunning;
};

LRESULT CALLBACK WindowProc(HWND window, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam);
void ResizeBitmap(int width, int height);
void RenderGradient(int xOffset, int yOffset);
void BlitBitmap(HDC deviceContext, RECT* windowRect);
StateInfo* GetAppState(HWND window);

int WINAPI WinMain(HINSTANCE instance, 
                   HINSTANCE prevInstance, 
                   PSTR cmdLine,
                   int cmdShow)
{
  StateInfo* appState = (StateInfo*)malloc(sizeof(StateInfo));
  appState->isRunning = true;
  // Register window class.

  const char CLASS_NAME[] = "Wend Class";

  WNDCLASS windowClass = {};
  windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = WindowProc;
  windowClass.hInstance = instance;
  windowClass.lpszClassName = CLASS_NAME;

  if (!RegisterClass(&windowClass))
  {
    return 0;
  }

  // Create window.

  HWND window = CreateWindowEx(
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
      &appState
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
    RenderGradient(xOffset, yOffset);

    HDC deviceContext = GetDC(window);
    RECT clientRect = {};
    GetClientRect(window, &clientRect);
    BlitBitmap(deviceContext, &clientRect);
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
  StateInfo* appState;
  if (message == WM_CREATE)
  {
    CREATESTRUCT* appCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
    appState = reinterpret_cast<StateInfo*>(appCreate->lpCreateParams);
    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)appState);
  }
  else
  {
    appState = GetAppState(window);
  }

  switch (message)
  {
    case WM_SIZE:
    {
      RECT clientRect = {};
      GetClientRect(window, &clientRect);
      int width = clientRect.right;
      int height = clientRect.bottom;
      ResizeBitmap(width, height);
      return 0;
    }
    case WM_ACTIVATEAPP:
    {
      return 0;
    }
    case WM_CLOSE:
    {
      if (MessageBox(window, "Are you sure you want to quit? Unsaved progress will be lost.", "Wend", MB_OKCANCEL) == IDOK)
      {
        DestroyWindow(window);
      }
      return 0;
    }
    case WM_DESTROY:
    {
      appState->isRunning = false;
      return 0;
    }
    case WM_PAINT:
    {
      PAINTSTRUCT painter;
      HDC deviceContext = BeginPaint(window, &painter);

      RECT clientRect = {};
      GetClientRect(window, &clientRect);
      BlitBitmap(deviceContext, &clientRect);
      EndPaint(window, &painter);
      return 0;
    }
  }
  return DefWindowProc(window, message, wParam, lParam);
}

void ResizeBitmap(int width, int height)
{
  if (bitmap)
  {
    VirtualFree(bitmap, 0, MEM_RELEASE);
  }

  bitmapWidth = width;
  bitmapHeight = height;
  
  bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
  bitmapInfo.bmiHeader.biWidth = bitmapWidth;
  bitmapInfo.bmiHeader.biHeight = -bitmapHeight;
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = BI_RGB;

  const int bytesPerPixel = 4;
  int bitmapSize = (bitmapWidth * bitmapHeight) * bytesPerPixel;
  
  bitmap = VirtualAlloc(0, bitmapSize, MEM_COMMIT, PAGE_READWRITE);
}

void BlitBitmap(HDC deviceContext, RECT* windowRect)
{
  int windowWidth = windowRect->right - windowRect->left;
  int windowHeight = windowRect->bottom - windowRect->top;
  StretchDIBits(deviceContext,
                0, 0, bitmapWidth, bitmapHeight,
                0, 0, windowWidth, windowHeight,
                bitmap, &bitmapInfo,
                DIB_RGB_COLORS, SRCCOPY);
}

void RenderGradient(int xOffset, int yOffset)
{
  int pitch = bitmapWidth*4;
  uint8_t* row = (uint8_t*)bitmap;
  for(int y = 0; y < bitmapHeight; ++y)
  {
    uint32_t* pixel = (uint32_t*)row;
    for(int x = 0; x < bitmapWidth; ++x)
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
  LONG_PTR userData = GetWindowLongPtr(window, GWLP_USERDATA);
  StateInfo* state = reinterpret_cast<StateInfo*>(userData);
  return state;
}