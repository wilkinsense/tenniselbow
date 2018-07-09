#pragma once

#include "GameEngine.h"
#include "Player.h"
#include "Ball.h"
#include "Net.h"
#include <SDL_gamecontroller.h>

struct SDL_Texture;
union SDL_Event;

class Game: public GameEngine
{
  friend class GameEngine;

  enum RoundState
  {
    ROUND_STATE_SERVING,
    ROUND_STATE_PLAY,
    ROUND_STATE_END,
  };

  enum GameState
  {
    GAME_STATE_PLAY,
    GAME_STATE_PAUSED
  };

public:
  ~Game();

protected:
  Game();

  void InitializeImpl();
  void UpdateImpl(float dt);
  void DrawImpl(SDL_Renderer *renderer, float dt);

  void HandleInput(SDL_Event *evt);
  void Reset();
  void CalculateDrawOrder(std::vector<GameObject *>& drawOrder);

  Player _player;
  Ball _ball;
  Net _net;

  SDL_Texture *_courtImage;

  // Using the default member-wise initializer for our new struct.
  Vector2 _servingDirection;

  RoundState _roundState;
  GameState _gameState;

  SDL_GameController *_controller;
};