#ifndef __WEND_INPUT_H__
#define __WEND_INPUT_H__

#include <cstdint>
#include <map>
#include <windows.h>
#include <xinput.h>

#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef XINPUT_GET_STATE(fn_XInputGetState);
/* XINPUT_GET_STATE(XInputGetStateStub)
{
  return 0;
} */

#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef XINPUT_SET_STATE(fn_XInputSetState);
/* XINPUT_SET_STATE(XInputSetStateStub)
{
  return 0;
} */

enum State
{
  IS_PRESSED = (1 << 0),
  WAS_PRESSED = (1 << 1), 
};

enum Key
{
  NONE = 0x00,

  // Command keys
  ESCAPE    = 0x01,
  ENTER     = 0x02,
  SPACE     = 0x03,
  BACKSPACE = 0x04,
  TAB       = 0x05,
  HOME      = 0x06,
  END       = 0x07,
  PAGEUP    = 0x08,
  PAGEDOWN  = 0x09,
  DEL       = 0x0A,
  INS       = 0x0B,
  PAUSE     = 0x0C,
  CAPSLOCK  = 0x0D,
  NUMLOCK   = 0x0E,
  SCROLL    = 0x0F,
  SUPER     = 0x10, // WINDOWS key
  MENU      = 0x11, // APPS key
  CLEAR     = 0x14,
  SHIFT     = 0x15,
  CTRL      = 0x16,
  ALT       = 0x17, // MENU key
  UP        = 0x18,
  DOWN      = 0x19,
  LEFT      = 0x1A,
  RIGHT     = 0x1B,
 
  // Operator Keys
  MULT      = 0x20,
  PLUS      = 0x21,
  MINUS     = 0x22,
  DIVIDE    = 0x23,
  DECM      = 0x24,
  HYPHEN    = 0x25,
  EQUAL     = 0x26,
  LBRACE    = 0x27,
  RBRACE    = 0x28,
  SLASH     = 0x29,
  BACKSLASH = 0x2A,
  COLON     = 0x2B,
  QUOTE     = 0x2C,
  COMMA     = 0x2D,
  PERIOD    = 0x2E,
  TILDE     = 0x2F,

  // Numeric Keys (Match ANSI)
  KP_0 = 0x30,
  KP_1 = 0x31,
  KP_2 = 0x32,
  KP_3 = 0x33,
  KP_4 = 0x34,
  KP_5 = 0x35,
  KP_6 = 0x36,
  KP_7 = 0x37,
  KP_8 = 0x38,
  KP_9 = 0x39,

  // Alpha keys (Match ANSI)
  A = 0x41,
  B = 0x42,
  C = 0x43,
  D = 0x44,
  E = 0x45,
  F = 0x46,
  G = 0x47,
  H = 0x48,
  I = 0x49,
  J = 0x4A,
  K = 0x4B,
  L = 0x4C,
  M = 0x4D,
  N = 0x4E,
  O = 0x4F,
  P = 0x50,
  Q = 0x51,
  R = 0x52,
  S = 0x53,
  T = 0x54,
  U = 0x55,
  V = 0x56,
  W = 0x57,
  X = 0x58,
  Y = 0x59,
  Z = 0x5A,

  // Numpad Keys
  NUM0 = 0x60,
  NUM1 = 0x61,
  NUM2 = 0x62,
  NUM3 = 0x63,
  NUM4 = 0x64,
  NUM5 = 0x65,
  NUM6 = 0x66,
  NUM7 = 0x67,
  NUM8 = 0x68,
  NUM9 = 0x69,
  
  // Function Keys
  F1  = 0x71,
  F2  = 0x72,
  F3  = 0x73,
  F4  = 0x74,
  F5  = 0x75,
  F6  = 0x76,
  F7  = 0x77,
  F8  = 0x78,
  F9  = 0x79,
  F10 = 0x7A,
  F11 = 0x7B,
  F12 = 0x7C,
  F13 = 0x7D,
  F14 = 0x7E,
  F15 = 0x7F,
  F16 = 0x80,
  F17 = 0x81,
  F18 = 0x82,
  F19 = 0x83,
  F20 = 0x84,
  F21 = 0x85,
  F22 = 0x86,
  F23 = 0x87,
  F24 = 0x88,
};

namespace Input
{

  struct Keyboard
  {
    uint8_t keyState[256];
    // creates a hashmap pairing between virtual key and custom keycode. 
    // cross-platform function
    static const inline std::map<size_t, Key> keyMap = {
      {0x00, Key::NONE}, 
      
      {'A', Key::A}, {'B', Key::B}, {'C', Key::C}, 
      {'D', Key::D}, {'E', Key::E}, {'F', Key::F},
      {'G', Key::G}, {'H', Key::H}, {'I', Key::I},
      {'J', Key::J}, {'K', Key::K}, {'L', Key::L},
      {'M', Key::M}, {'N', Key::N}, {'O', Key::O},
      {'P', Key::P}, {'Q', Key::Q}, {'R', Key::R},
      {'S', Key::S}, {'T', Key::T}, {'U', Key::U},
      {'V', Key::V}, {'W', Key::W}, {'X', Key::X},
      {'Y', Key::Y}, {'Z', Key::Z},

      {'0', Key::KP_0}, {'1', Key::KP_1},
      {'2', Key::KP_2}, {'3', Key::KP_3},
      {'4', Key::KP_4}, {'5', Key::KP_5},
      {'6', Key::KP_6}, {'7', Key::KP_7},
      {'8', Key::KP_8}, {'9', Key::KP_9},

      {VK_NUMPAD0, Key::NUM0}, {VK_NUMPAD1, Key::NUM1},
      {VK_NUMPAD2, Key::NUM2}, {VK_NUMPAD3, Key::NUM3},
      {VK_NUMPAD4, Key::NUM4}, {VK_NUMPAD5, Key::NUM5},
      {VK_NUMPAD6, Key::NUM6}, {VK_NUMPAD7, Key::NUM7},
      {VK_NUMPAD8, Key::NUM8}, {VK_NUMPAD9, Key::NUM9},

      {VK_F1, Key::F1},   {VK_F2, Key::F2},
      {VK_F3, Key::F3},   {VK_F4, Key::F4},
      {VK_F5, Key::F5},   {VK_F6, Key::F6},
      {VK_F7, Key::F7},   {VK_F8, Key::F8},
      {VK_F9, Key::F9},   {VK_F10, Key::F10},
      {VK_F11, Key::F11}, {VK_F12, Key::F12},
      {VK_F11, Key::F13}, {VK_F12, Key::F14},
      {VK_F11, Key::F15}, {VK_F12, Key::F16},
      {VK_F11, Key::F17}, {VK_F12, Key::F18},
      {VK_F11, Key::F19}, {VK_F12, Key::F20},
      {VK_F11, Key::F21}, {VK_F12, Key::F22},
      {VK_F11, Key::F23}, {VK_F12, Key::F24},

      {VK_MULTIPLY, Key::MULT}, {VK_ADD, Key::PLUS}, 
      {VK_DIVIDE, Key::DIVIDE}, {VK_SUBTRACT, Key::MINUS}, 
      {VK_DECIMAL, Key::DECM},

      {VK_BACK, Key::BACKSPACE}, {VK_ESCAPE, Key::ESCAPE}, 
      {VK_RETURN, Key::ENTER},   {VK_TAB, Key::TAB}, 
      {VK_DELETE, Key::DEL},     {VK_HOME, Key::HOME},
      {VK_END, Key::END},        {VK_PRIOR, Key::PAGEUP}, 
      {VK_NEXT, Key::PAGEDOWN},  {VK_INSERT, Key::INS},
      {VK_SHIFT, Key::SHIFT},    {VK_CONTROL, Key::CTRL},
      {VK_SPACE, Key::SPACE},    {VK_NUMLOCK, Key::NUMLOCK},
      {VK_PAUSE, Key::PAUSE},    {VK_CAPITAL, Key::CAPSLOCK},
      {VK_SCROLL, Key::SCROLL},  {VK_MENU, Key::ALT},
      {VK_LWIN, Key::SUPER},     {VK_RWIN, Key::SUPER},
      {VK_APPS, Key::MENU},

      {VK_OEM_1, Key::COLON},       {VK_OEM_2, Key::SLASH},
      {VK_OEM_3, Key::TILDE},       {VK_OEM_4, Key::LBRACE},
      {VK_OEM_5, Key::BACKSLASH},   {VK_OEM_6, Key::RBRACE},
      {VK_OEM_7, Key::QUOTE},       {VK_OEM_PLUS, Key::EQUAL},
      {VK_OEM_COMMA, Key::COMMA},   {VK_OEM_MINUS, Key::MINUS},
      {VK_OEM_PERIOD, Key::PERIOD}, {VK_CAPITAL, Key::CAPSLOCK},

      {VK_UP, Key::UP},     {VK_DOWN, Key::DOWN}, 
      {VK_LEFT, Key::LEFT}, {VK_RIGHT, Key::RIGHT},
    };
  };

  bool CheckKeyIsPressed(uint8_t);
  bool CheckKeyWasPressed(uint8_t);
  bool CheckKeyIsHeld(uint8_t);
  bool CheckKeyIsJustPressed(uint8_t);
  bool CheckKeyIsReleased(uint8_t);
  bool CheckKeyIsJustReleased(uint8_t);

  void PoolKeyState(uint8_t*);

  void InitXInput(fn_XInputGetState**, fn_XInputSetState**);
}

#endif //__WEND_INPUT_H__