#pragma once

#include "GameEngine.h"
#include "Player.h"
#include "Ball.h"
#include "Net.h"

struct SDL_Texture;

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

  void Reset();
  void CalculateDrawOrder(std::vector<GameObject *>& drawOrder);

  Player _player;
  Ball _ball;
  Net _net;

  SDL_Texture *_court;

  // Using the default member-wise initializer for our new struct.
  Vector2 pos;
  Vector2 endPointOffset;
  float speed;
  float rotationSpeed;
  bool _ballInPlay;
  bool _hitNet;
};