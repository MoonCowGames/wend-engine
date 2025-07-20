#ifndef __WEND_WIN_INPUT_H__
#define __WEND_WIN_INPUT_H__


#include <windows.h>

#include "../../input/input.h"

namespace Win32
{
  /// @brief Facade for Windows input to ndependent translations
  /// @param winKey WPARAM 
  /// @return 
  Key TranslateInput(WPARAM winKey);
}


#endif //__WEND_WIN_INPUT_H__