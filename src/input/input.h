#ifndef __WEND_INPUT_H__
#define __WEND_INPUT_H__

#include <cstdint>
#include <map>
#include <windows.h>


enum Key
{
  NONE = 0x00,

  // Command keys
  ESCAPE    = 0x01,
  ENTER     = 0x02,
  SPACE     = 0x03,
  BACKSPACE = 0x04,
  TAB       = 0x05,
  CAPSLOCK  = 0x06,
  SHIFT     = 0x07,
  CTRL      = 0x08,
  ALT       = 0x09,
  HOME      = 0x0A,
  END       = 0x0B,
  PAGEUP    = 0x0C,
  PAGEDOWN  = 0x0D,
  DEL       = 0x0E,
  INS       = 0x0F,

  // Operator Keys
  MULT      = 0x10,
  PLUS      = 0x11,
  MINUS     = 0x12,
  DIVIDE    = 0x13,
  DEC       = 0x14,
  HYPHEN    = 0x15,
  EQUAL     = 0x16,
  LBRACE    = 0x17,
  RBRACE    = 0x18,
  SLASH     = 0x19,
  BACKSLASH = 0x1A,
  COLON     = 0x1B,
  QUOTE     = 0x1C,
  COMMA     = 0x1D,
  PERIOD    = 0x1E,
  TILDE     = 0x1F,

  // Function Keys
  F1  = 0x21,
  F2  = 0x22,
  F3  = 0x23,
  F4  = 0x24,
  F5  = 0x25,
  F6  = 0x26,
  F7  = 0x27,
  F8  = 0x28,
  F9  = 0x29,
  F10 = 0x2A,
  F11 = 0x2B,
  F12 = 0x2C,
  
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
};

static const std::map<size_t, Key> keyMap = {
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

      {VK_MULTIPLY, Key::MULT}, {VK_ADD, Key::PLUS}, 
      {VK_DIVIDE, Key::DIVIDE}, {VK_SUBTRACT, Key::MINUS}, 
      {VK_DECIMAL, Key::DEC},

      {VK_BACK, Key::BACKSPACE}, {VK_ESCAPE, Key::ESCAPE}, 
      {VK_RETURN, Key::ENTER},   {VK_TAB, Key::TAB}, 
      {VK_DELETE, Key::DEL},     {VK_HOME, Key::HOME},
      {VK_END, Key::END},        {VK_PRIOR, Key::PAGEUP}, 
      {VK_NEXT, Key::PAGEDOWN},  {VK_INSERT, Key::INS},
      {VK_SHIFT, Key::SHIFT},    {VK_CONTROL, Key::CTRL},
      {VK_SPACE, Key::SPACE},

      {VK_OEM_1, Key::COLON},       {VK_OEM_2, Key::SLASH},
      {VK_OEM_3, Key::TILDE},       {VK_OEM_4, Key::LBRACE},
      {VK_OEM_5, Key::BACKSLASH},   {VK_OEM_6, Key::RBRACE},
      {VK_OEM_7, Key::QUOTE},       {VK_OEM_PLUS, Key::EQUAL},
      {VK_OEM_COMMA, Key::COMMA},   {VK_OEM_MINUS, Key::MINUS},
      {VK_OEM_PERIOD, Key::PERIOD}, {VK_CAPITAL, Key::CAPSLOCK},
};

namespace Input
{

  struct Keyboard
  {
    // creates a hashmap pairing between virtual key and custom keycode. 
    // cross-platform function
    
    uint8_t keyState[256];
  };

  /* void SetKeyMap(std::map<size_t, uint8_t>); */
  void SetKeyState(uint8_t[256], int32_t, uint8_t);
  uint8_t GetKeyState(uint8_t*, uint8_t);
  bool CheckKeyIsDown(uint8_t);
  bool CheckKeyWasDown(uint8_t);
}

#endif //__WEND_INPUT_H__