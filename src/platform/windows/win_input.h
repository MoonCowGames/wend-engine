#ifndef __WEND_WIN_INPUT_H__
#define __WEND_WIN_INPUT_H__


#include <windows.h>

#include "../../input/input.h"

namespace Win32
{
  /// @brief Facade for Windows keyboard input to ndependent translations
  /// @param winKey WPARAM of KeyDown/KeyUp Windows message
  /// @return Translated keycode
  Key TranslateKeyboard(WPARAM winKey);
}

#endif //__WEND_WIN_INPUT_H__