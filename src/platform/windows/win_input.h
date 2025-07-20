#ifndef __WEND_WIN_INPUT_H__
#define __WEND_WIN_INPUT_H__


#include <windows.h>

#include "../../input/input.h"

/// @brief Facade for Windows input to ndependent translations
/// @param winKey WPARAM 
/// @return 
Key TranslateInput(WPARAM winKey)
{
  switch (winKey)
  {
    // Alphanumeric
    case 'A':           return Key::A;
    case 'B':           return Key::B;
    case 'C':           return Key::C;
    case 'D':           return Key::D;
    case 'E':           return Key::E;
    case 'F':           return Key::F;
    case 'G':           return Key::G;
    case 'H':           return Key::H;
    case 'I':           return Key::I;
    case 'J':           return Key::J;
    case 'K':           return Key::K;
    case 'L':           return Key::L;
    case 'M':           return Key::M;
    case 'N':           return Key::N;
    case 'O':           return Key::O;
    case 'P':           return Key::P;
    case 'Q':           return Key::Q;
    case 'R':           return Key::R;
    case 'S':           return Key::S;
    case 'T':           return Key::T;
    case 'U':           return Key::U;
    case 'V':           return Key::V;
    case 'W':           return Key::W;
    case 'X':           return Key::X;
    case 'Y':           return Key::Y;
    case 'Z':           return Key::Z;
    case '0':           return Key::KP_0;
    case '1':           return Key::KP_1;
    case '2':           return Key::KP_2;
    case '3':           return Key::KP_3;
    case '4':           return Key::KP_4;
    case '5':           return Key::KP_5;
    case '6':           return Key::KP_6;
    case '7':           return Key::KP_7;
    case '8':           return Key::KP_8;
    case '9':           return Key::KP_9;

    // Numpad Numeric
    case VK_NUMPAD0:    return Key::NUM0;
    case VK_NUMPAD1:    return Key::NUM1;
    case VK_NUMPAD2:    return Key::NUM2;
    case VK_NUMPAD3:    return Key::NUM3;
    case VK_NUMPAD4:    return Key::NUM4;
    case VK_NUMPAD5:    return Key::NUM5;
    case VK_NUMPAD6:    return Key::NUM6;
    case VK_NUMPAD7:    return Key::NUM7;
    case VK_NUMPAD8:    return Key::NUM8;
    case VK_NUMPAD9:    return Key::NUM9;
    
    // Numpad Operators
    case VK_MULTIPLY:   return Key::NUM_MULT;
    case VK_ADD:        return Key::NUM_PLUS;
    case VK_DIVIDE:     return Key::NUM_DIVIDE;
    case VK_SUBTRACT:   return Key::NUM_MINUS;
    case VK_DECIMAL:    return Key::NUM_DECIMAL;

    // Function keys
    case VK_F1:         return Key::F1;
    case VK_F2:         return Key::F2;
    case VK_F3:         return Key::F3;
    case VK_F4:         return Key::F4;
    case VK_F5:         return Key::F5;
    case VK_F6:         return Key::F6;
    case VK_F7:         return Key::F7;
    case VK_F8:         return Key::F8;
    case VK_F9:         return Key::F9;
    case VK_F10:        return Key::F10;
    case VK_F11:        return Key::F11;
    case VK_F12:        return Key::F12;
    case VK_F13:        return Key::F13;
    case VK_F14:        return Key::F14;
    case VK_F15:        return Key::F15;
    case VK_F16:        return Key::F16;
    case VK_F17:        return Key::F17;
    case VK_F18:        return Key::F18;
    case VK_F19:        return Key::F19;
    case VK_F20:        return Key::F20;
    case VK_F21:        return Key::F21;
    case VK_F22:        return Key::F22;
    case VK_F23:        return Key::F23;
    case VK_F24:        return Key::F24;
    
    // OEM keys
    case VK_OEM_1:      return Key::COLON;
    case VK_OEM_2:      return Key::SLASH;
    case VK_OEM_3:      return Key::TILDE;
    case VK_OEM_4:      return Key::LBRACE;
    case VK_OEM_5:      return Key::BACKSLASH;
    case VK_OEM_6:      return Key::RBRACE;
    case VK_OEM_7:      return Key::QUOTE;
    case VK_OEM_PLUS:   return Key::EQUAL;
    case VK_OEM_COMMA:  return Key::COMMA;
    case VK_OEM_MINUS:  return Key::HYPHEN;
    case VK_OEM_PERIOD: return Key::PERIOD;

    // Arrow keys
    case VK_UP:         return Key::UP;
    case VK_DOWN:       return Key::DOWN;
    case VK_LEFT:       return Key::LEFT;
    case VK_RIGHT:      return Key::RIGHT;
    
    // Command keys
    case VK_BACK:       return Key::BACKSPACE;
    case VK_ESCAPE:     return Key::ESCAPE;
    case VK_RETURN:     return Key::ENTER;
    case VK_TAB:        return Key::TAB;
    case VK_DELETE:     return Key::DEL;
    case VK_HOME:       return Key::HOME;
    case VK_END:        return Key::END;
    case VK_PRIOR:      return Key::PAGEUP;
    case VK_NEXT:       return Key::PAGEDOWN;
    case VK_INSERT:     return Key::INS;
    case VK_SHIFT:      return Key::SHIFT;
    case VK_CONTROL:    return Key::CTRL;
    case VK_SPACE:      return Key::SPACE;
    case VK_NUMLOCK:    return Key::NUMLOCK;
    case VK_PAUSE:      return Key::PAUSE;
    case VK_CAPITAL:    return Key::CAPSLOCK;
    case VK_SCROLL:     return Key::SCROLL;
    case VK_MENU:       return Key::ALT;
    case VK_LWIN:       return Key::SUPER;
    case VK_RWIN:       return Key::SUPER;
    case VK_APPS:       return Key::MENU;
    
    // Unassigned
    default:            return Key::NONE;
  }
}

#endif //__WEND_WIN_INPUT_H__