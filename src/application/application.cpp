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
  // TODO: Give dev control of pos and size
  app->xPos = 0;
  app->yPos = 0;
  app->width = 1280;
  app->height = 720;
  
  app->keyboard = {0};
  app->mouse = {0};
  for (int i = 0; i < MAX_CONTROLLERS; i++)
  {
    app->gamepad[i] = {0};
  }
  
  // TODO: FrameBuffer should init here. Should make request to platform layer to alloc.
  // TODO: Soundbuffer should also make request to platform layer to alloc.
  Sound::InitSoundBuffer(&(app->soundBuffer), &(app->soundCfg));
  
  app->isRunning = true;
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
  
  app->mouse.wheelDelta = 0;
}