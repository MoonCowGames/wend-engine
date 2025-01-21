#ifndef __WEND_AUDIO_H__
#define __WEND_AUDIO_H__

#include <windows.h>
#include <dsound.h>
#include <cstdint>
#include <iostream>

namespace Audio
{
  void InitDirectSound(IDirectSoundBuffer**, HWND, uint32_t);
}

#endif //__WEND_AUDIO_H__