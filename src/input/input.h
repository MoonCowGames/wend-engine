/*
+------------------------------------------------------------------------------+
|File: input.h                                                                 |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#ifndef __WEND_INPUT_H__
#define __WEND_INPUT_H__

#include <windows.h>
#include <xinput.h>

#include "../misc/includes.h"

/// @brief Maximum number of keys to track state of
/// @note 128 maximum - Full sized keyboards have only 107 keys 
const int32 MAX_KEYBOARD_SIZE = 128;

const int32 MAX_CONTROLLERS = 4;

enum State
{
  IS_PRESSED = (1 << 0),
  WAS_PRESSED = (1 << 1), 
};

/// @brief Maps int values to name of keyboard keys
/// @note Should additional keys be added, cannot exceed 0xFF - must fit in 256 size array
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
  NUM_MULT      = 0x1C,
  NUM_PLUS      = 0x1D,
  NUM_MINUS     = 0x1E,
  NUM_DIVIDE    = 0x1F,
  NUM_DECIMAL   = 0x20,
  HYPHEN    = 0x21,
  EQUAL     = 0x22,
  LBRACE    = 0x23,
  RBRACE    = 0x24,
  SLASH     = 0x25,
  BACKSLASH = 0x26,
  COLON     = 0x27,
  QUOTE     = 0x28,
  COMMA     = 0x29,
  PERIOD    = 0x2A,
  TILDE     = 0x2B,

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
  
  // Function Keys F1-F12
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

  // Function Keys F13-F24 (unaligned to pack into max size of 128)
  F13 = 0x2C,
  F14 = 0x2D,
  F15 = 0x2E,
  F16 = 0x2F,
  F17 = 0x3A,
  F18 = 0x3B,
  F19 = 0x3C,
  F20 = 0x3D,
  F21 = 0x3E,
  F22 = 0x3F,
  F23 = 0x40,
  F24 = 0x5B,
};

namespace Input
{
  struct Keyboard
  {
    /// @brief Uses 8 bit bitfield to determine state (6 bits unused)
    uint8 keyState[MAX_KEYBOARD_SIZE] = {0};
  };

  struct Mouse
  {
    int32 xPos = 0;
    int32 yPos = 0;
    int32 wheelDelta = 0;

    uint8 leftButton = 0;
    uint8 rightButton = 0;
    uint8 middleButton = 0;
    uint8 thumb1Button = 0;
    uint8 thumb2Button = 0;
  };

  struct Gamepad
  {
    uint8 dpadUp = 0;
    uint8 dpadDown = 0;
    uint8 dpadLeft = 0;
    uint8 dpadRight = 0;
    uint8 faceBottom = 0;
    uint8 faceRight = 0;
    uint8 faceLeft = 0;
    uint8 faceTop = 0;
    uint8 shoulderLeft = 0;
    uint8 shoulderRight = 0;
    uint8 thumbstickLeft = 0;
    uint8 thumbstickRight = 0;
    uint8 start = 0;
    uint8 select;

    uint8 triggerLeft = 0;
    uint8 triggerRight = 0;

    int16 xAxisLeft = 0;
    int16 yAxisLeft = 0;
    
    int16 xAxisRight = 0;
    int16 yAxisRight = 0;
  };

      ///@param state Input state bitfield.
      ///@return Returns true if key is pressed, else false.
  bool IsPressed(uint8 state);

      ///@param state Input state bitfield.
      ///@return Returns true if key was pressed, else false.
  bool WasPressed(uint8 state);

      ///@param state Input state bitfield.
      ///@return Returns true only if key is pressed this frame but last frame it was not, else false.
  bool IsJustPressed(uint8 state);

      ///@param state Input state bitfield.
      ///@return Returns true if key is released, else false.
  bool IsReleased(uint8 state);

      ///@param state Input state bitfield.
      ///@return Returns true only if key is released this frame but last frame it was not, else false.
  bool IsJustReleased(uint8 state);

      ///@param state Input state bitfield.
      ///@return Returns true if key was released, else false.
  bool CheckKeyWasReleased(uint8 state);

      ///@brief Iterates keys and updates with changes.
      ///@param keyState Array of input state bitfields.
  void PoolKeyState(uint8* state);
}

#endif //__WEND_INPUT_H__