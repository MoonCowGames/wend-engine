/*
+------------------------------------------------------------------------------+
|File: input.cpp                                                               |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "input.h"

bool Input::IsPressed(uint8 state)
{
  return (state & State::IS_PRESSED) != 0;
}

bool Input::WasPressed(uint8 state)
{
  return (state & State::WAS_PRESSED) != 0;
}

bool Input::IsJustPressed(uint8 state)
{
  return IsPressed(state) && !WasPressed(state);
}

bool Input::IsReleased(uint8 state)
{
  return (state & State::IS_PRESSED) == 0;
}

bool Input::IsJustReleased(uint8 state)
{
  return IsReleased(state) && !CheckKeyWasReleased(state);
}

bool Input::CheckKeyWasReleased(uint8 state)
{
  return (state & State::WAS_PRESSED) == 0;
}

void Input::PoolKeyState(uint8* keyState)
{
  // TODO: Consider rethinking this. Queue keys on state change?
  for (int keyIndex = 0; keyIndex < 256; keyIndex++)
  {
    if (IsJustPressed(keyState[(Key)keyIndex]))
    {
      keyState[(Key)keyIndex] |= State::WAS_PRESSED;
    }
    else if (IsJustReleased(keyState[(Key)keyIndex]))
    {
      keyState[(Key)keyIndex] &= ~(State::WAS_PRESSED);
    }
  }
}