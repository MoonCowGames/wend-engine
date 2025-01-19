#include "input.h"

namespace Input
{
  bool CheckKeyIsPressed(uint8_t state)
  {
    return (state & (1 << 0)) != 0;
  }
  bool CheckKeyWasPressed(uint8_t state)
  {
    return (state & (1 << 1)) != 0;
  }
  bool CheckKeyIsHeld(uint8_t state)
  {
    return CheckKeyIsPressed(state) && CheckKeyWasPressed(state);
  }
  bool CheckKeyIsJustPressed(uint8_t state)
  {
    return CheckKeyIsPressed(state) && !CheckKeyWasPressed(state);
  }
  bool CheckKeyIsReleased(uint8_t state)
  {
    return (state & (1 << 0)) == 0;
  }
  bool CheckKeyWasReleased(uint8_t state)
  {
    return (state & (1 << 1)) == 0;
  }
  bool CheckKeyIsJustReleased(uint8_t state)
  {
    return CheckKeyIsReleased(state) && !CheckKeyWasReleased(state);
  }
}