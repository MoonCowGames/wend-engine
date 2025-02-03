#include "input.h"

namespace Input
{
  bool CheckKeyIsPressed(uint8_t state)
  {
    return (state & State::IS_PRESSED) != 0;
  }
  bool CheckKeyWasPressed(uint8_t state)
  {
    return (state & State::WAS_PRESSED) != 0;
  }
  bool CheckKeyIsJustPressed(uint8_t state)
  {
    return CheckKeyIsPressed(state) && !CheckKeyWasPressed(state);
  }
  bool CheckKeyIsReleased(uint8_t state)
  {
    return (state & State::IS_PRESSED) == 0;
  }
  bool CheckKeyWasReleased(uint8_t state)
  {
    return (state & State::WAS_PRESSED) == 0;
  }
  bool CheckKeyIsJustReleased(uint8_t state)
  {
    return CheckKeyIsReleased(state) && !CheckKeyWasReleased(state);
  }

  void PoolKeyState(uint8_t* keyState)
  {
    for (int keyIndex = 0; keyIndex < 256; keyIndex++)
    {
      if (Input::CheckKeyIsJustPressed(keyState[(Key)keyIndex]))
      {
        keyState[(Key)keyIndex] |= State::WAS_PRESSED;
      }
      else if (Input::CheckKeyIsJustReleased(keyState[(Key)keyIndex]))
      {
        keyState[(Key)keyIndex] &= ~(State::WAS_PRESSED);
      }
    }
  }
  
  void InitXInput(fn_XInputGetState** XInputGetState, fn_XInputSetState** XInputSetState)
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
}