#pragma once

#include <GameEngine.h>
#include <ScreenManager.h>
#include <SDL_gamecontroller.h>

#define NEW_SYSTEM

class Game: public GameEngine
{
  friend class GameEngine;

public:
  ~Game();

protected:
  Game();

  void InitializeImpl();
  void UpdateImpl(float dt);
  void DrawImpl(SDL_Renderer *renderer, float dt);

  SDL_GameController *_controller;
  ScreenManager *_screenManager;
};