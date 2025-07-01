/*
+------------------------------------------------------------------------------+
|File: application.cpp                                                         |
|Author: Luna Artemis Dorn                                                     |
|Notice: (C) Copyright 2025 of Luna Artemis Dorn. All Rights Reserved.         |
+------------------------------------------------------------------------------+
*/

#include "application.h"

void App::InitApplication(Application* app)
{
  app->isRunning = true;
  
  for (int index = 0; index < 256; index++)
  {
    app->keyboard.keyState[index] = 0;
  }

  Sound::InitSoundBuffer(&(app->soundBuffer), &(app->soundCfg));
}

void App::FrameUpdate(Application* app, float32 deltaTime)
{
  // TODO: Process per-frame changes.
  Sound::FillSoundBuffer(&(app->soundBuffer), &(app->soundCfg));
}