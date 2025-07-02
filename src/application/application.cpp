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
  static int xOffset = 0;
  static int yOffset = 0;

  uint8* keyState = app->keyboard.keyState;
  Input::PoolKeyState(keyState);

  if (Input::IsPressed(keyState[Key::W]) ||
      Input::IsPressed(keyState[Key::UP]))
  {
    yOffset++;
  }
  if (Input::IsPressed(keyState[Key::S]) ||
      Input::IsPressed(keyState[Key::DOWN]))
  {
    yOffset--;
  }
  if (Input::IsPressed(keyState[Key::A]) ||
      Input::IsPressed(keyState[Key::LEFT]))
  {
    xOffset++;
  }
  if (Input::IsPressed(keyState[Key::D]) ||
      Input::IsPressed(keyState[Key::RIGHT]))
  {
    xOffset--;
  }

  // TODO: Test controller in Application

  Render::RenderGradient(&(app->frameBuffer), xOffset, yOffset);

  Sound::FillSoundBuffer(&(app->soundBuffer), &(app->soundCfg));
}