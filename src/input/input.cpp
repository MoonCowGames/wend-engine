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
  bool IsPressed(uint8 state)
  {
    return (state & State::IS_PRESSED) != 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true if key was pressed, else false.
  bool WasPressed(uint8 state)
  {
    return (state & State::WAS_PRESSED) != 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true only if key is pressed this frame but last frame it was not, else false.
  bool IsJustPressed(uint8 state)
  {
    return IsPressed(state) && !WasPressed(state);
  }

  ///@param state Input state bitfield.
  ///@return Returns true if key is released, else false.
  bool IsReleased(uint8 state)
  {
    return (state & State::IS_PRESSED) == 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true if key was released, else false.
  bool CheckKeyWasReleased(uint8 state)
  {
    return (state & State::WAS_PRESSED) == 0;
  }

  ///@param state Input state bitfield.
  ///@return Returns true only if key is released this frame but last frame it was not, else false.
  bool IsJustReleased(uint8 state)
  {
    return IsReleased(state) && !CheckKeyWasReleased(state);
  }

  // TODO: Consider rethinking this. Queue keys on state change?
  /**
   * Loops through all indexes in the keyState array and updates the WAS_PRESSED
   * state of the key to reflect any changes.
   * 
   * @param keyState Array of input state bitfields.
   */
  void PoolKeyState(uint8* keyState)
  {
    for (int keyIndex = 0; keyIndex < 256; keyIndex++)
    {
      if (Input::IsJustPressed(keyState[(Key)keyIndex]))
      {
        keyState[(Key)keyIndex] |= State::WAS_PRESSED;
      }
      else if (Input::IsJustReleased(keyState[(Key)keyIndex]))
      {
        keyState[(Key)keyIndex] &= ~(State::WAS_PRESSED);
      }
    }
  }
  
}