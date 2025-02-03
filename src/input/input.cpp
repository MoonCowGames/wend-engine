/*
+------------------------------------------------------------------------------+
|File: input.cpp                                                               |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "input.h"

namespace Input
{
  ///@param state Input state bitfield.
  ///@return Returns true if key is pressed, else false.
  bool CheckKeyIsPressed(uint8_t state)
  {
    return (state & State::IS_PRESSED) != 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true if key was pressed, else false.
  bool CheckKeyWasPressed(uint8_t state)
  {
    return (state & State::WAS_PRESSED) != 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true only if key is pressed this frame but last frame it was not, else false.
  bool CheckKeyIsJustPressed(uint8_t state)
  {
    return CheckKeyIsPressed(state) && !CheckKeyWasPressed(state);
  }

  ///@param state Input state bitfield.
  ///@return Returns true if key is released, else false.
  bool CheckKeyIsReleased(uint8_t state)
  {
    return (state & State::IS_PRESSED) == 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true if key was released, else false.
  bool CheckKeyWasReleased(uint8_t state)
  {
    return (state & State::WAS_PRESSED) == 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true only if key is released this frame but last frame it was not, else false.
  bool CheckKeyIsJustReleased(uint8_t state)
  {
    return CheckKeyIsReleased(state) && !CheckKeyWasReleased(state);
  }

  /**
   * Loops through all indexes in the keyState array and updates the WAS_PRESSED
   * state of the key to reflect any changes.
   * 
   * @param keyState Array of input state bitfields.
   */
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