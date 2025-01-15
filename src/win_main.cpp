#include <windows.h>

LRESULT CALLBACK WindowProc(HWND window, 
                            UINT message, 
                            WPARAM wParam, 
                            LPARAM lParam);

int APIENTRY WINAPI WinMain(HINSTANCE instance, 
                            HINSTANCE prevInstance, 
                            PSTR cmdLine,
                            int cmdShow)
{
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
      NULL
  );

  if (window == NULL)
  {
    return 0;
  }

  ShowWindow(window, cmdShow);
  UpdateWindow(window);

  MSG message = {};
  int msgResult = 0;
  while (msgResult = GetMessage(&message, NULL, 0, 0))
  {
    if (msgResult == 0)
    {
      break;
    }
    else if (msgResult == -1)
    {
      // TODO: Error handling
      break;
    }
    else 
    {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }
  return 0;
}

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_CREATE:
    {
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
      PostQuitMessage(0);
      return 0;
    }

    case WM_PAINT:
    {
      PAINTSTRUCT painter;
      HDC deviceContext = BeginPaint(window, &painter);

      // All painting occurs here, between BeginPaint and EndPaint.

      FillRect(deviceContext, &painter.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

      EndPaint(window, &painter);
      return 0;
    }
  }
  return DefWindowProc(window, message, wParam, lParam);
}
