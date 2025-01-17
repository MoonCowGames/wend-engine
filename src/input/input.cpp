#include "input.h"

namespace Input
{
  /* void SetKeyMap(std::map<size_t, uint8_t> keyMap)
  {
    keyMap[0x00] = Key::NONE;

    keyMap['A'] = Key::A; keyMap['B'] = Key::B; keyMap['C'] = Key::C; 
    keyMap['D'] = Key::D; keyMap['E'] = Key::E; keyMap['F'] = Key::F;
    keyMap['G'] = Key::G; keyMap['H'] = Key::H; keyMap['I'] = Key::I;
    keyMap['J'] = Key::J; keyMap['K'] = Key::K; keyMap['L'] = Key::L;
    keyMap['M'] = Key::M; keyMap['N'] = Key::N; keyMap['O'] = Key::O;
    keyMap['P'] = Key::P; keyMap['Q'] = Key::Q; keyMap['R'] = Key::R;
    keyMap['S'] = Key::S; keyMap['T'] = Key::T; keyMap['U'] = Key::U;
    keyMap['V'] = Key::V; keyMap['W'] = Key::W; keyMap['X'] = Key::X;
    keyMap['Y'] = Key::Y; keyMap['Z'] = Key::Z;

    keyMap['0'] = Key::KP_0; keyMap['1'] = Key::KP_1;
    keyMap['2'] = Key::KP_2; keyMap['3'] = Key::KP_3;
    keyMap['4'] = Key::KP_4; keyMap['5'] = Key::KP_5;
    keyMap['6'] = Key::KP_6; keyMap['7'] = Key::KP_7;
    keyMap['8'] = Key::KP_8; keyMap['9'] = Key::KP_9;

    keyMap[VK_NUMPAD0] = Key::NUM0; keyMap[VK_NUMPAD1] = Key::NUM1;
    keyMap[VK_NUMPAD2] = Key::NUM2; keyMap[VK_NUMPAD3] = Key::NUM3;
    keyMap[VK_NUMPAD4] = Key::NUM4; keyMap[VK_NUMPAD5] = Key::NUM5;
    keyMap[VK_NUMPAD6] = Key::NUM6; keyMap[VK_NUMPAD7] = Key::NUM7;
    keyMap[VK_NUMPAD8] = Key::NUM8; keyMap[VK_NUMPAD9] = Key::NUM9;

    keyMap[VK_F1] = Key::F1;   keyMap[VK_F2] = Key::F2;
    keyMap[VK_F3] = Key::F3;   keyMap[VK_F4] = Key::F4;
    keyMap[VK_F5] = Key::F5;   keyMap[VK_F6] = Key::F6;
    keyMap[VK_F7] = Key::F7;   keyMap[VK_F8] = Key::F8;
    keyMap[VK_F9] = Key::F9;   keyMap[VK_F10] = Key::F10;
    keyMap[VK_F11] = Key::F11; keyMap[VK_F12] = Key::F12;

    keyMap[VK_MULTIPLY] = Key::MULT; keyMap[VK_ADD] = Key::PLUS; 
    keyMap[VK_DIVIDE] = Key::DIVIDE;   keyMap[VK_SUBTRACT] = Key::MINUS; 
    keyMap[VK_DECIMAL] = Key::DEC;

    keyMap[VK_BACK] = Key::BACKSPACE; keyMap[VK_ESCAPE] = Key::ESCAPE; 
    keyMap[VK_RETURN] = Key::ENTER; keyMap[VK_TAB] = Key::TAB; 
    keyMap[VK_DELETE] - Key::DEL; keyMap[VK_HOME] = Key::HOME;
		keyMap[VK_END] = Key::END; keyMap[VK_PRIOR] = Key::PAGEUP; 
    keyMap[VK_NEXT] = Key::PAGEDOWN; keyMap[VK_INSERT] = Key::INS;
    keyMap[VK_SHIFT] = Key::SHIFT; keyMap[VK_CONTROL] = Key::CTRL;
		keyMap[VK_SPACE] = Key::SPACE;

    keyMap[VK_OEM_1] = Key::COLON;
		keyMap[VK_OEM_2] = Key::SLASH;
		keyMap[VK_OEM_3] = Key::TILDE;
		keyMap[VK_OEM_4] = Key::LBRACE;
		keyMap[VK_OEM_5] = Key::BACKSLASH;
		keyMap[VK_OEM_6] = Key::RBRACE;
		keyMap[VK_OEM_7] = Key::QUOTE;
		keyMap[VK_OEM_PLUS] = Key::EQUAL;
		keyMap[VK_OEM_COMMA] = Key::COMMA;
		keyMap[VK_OEM_MINUS] = Key::MINUS;
		keyMap[VK_OEM_PERIOD] = Key::PERIOD;
		keyMap[VK_CAPITAL] = Key::CAPSLOCK;
  } */

  void SetKeyState(uint8_t keyState[256], int32_t key, uint8_t state)
  {
    keyState[key] = state;
  }

  uint8_t GetKeyState(uint8_t* keyState, uint8_t key)
  {
    return keyState[key];
  }

  bool CheckKeyIsDown(uint8_t state)
  {
    return (state & (1 << 0));
  }
  bool CheckKeyWasDown(uint8_t state)
  {
    return (state & (1 << 1));
  }
}