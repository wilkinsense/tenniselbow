#pragma once

#include "GameEngine.h"
#include "Player.h"
#include "Ball.h"

class Game: public GameEngine
{
  friend class GameEngine;

protected:
  Game();

  void InitializeImpl();
  void UpdateImpl(float dt);
  void DrawImpl(SDL_Renderer *renderer, float dt);

  Player _player;
  Ball _ball;

  // Using the default member-wise initializer for our new struct.
  Vector2 pos;
  Vector2 endPointOffset;
  float speed;
  float rotationSpeed;
};