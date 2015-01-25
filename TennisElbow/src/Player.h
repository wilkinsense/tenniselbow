#pragma once

#include "GameObject.h"
#include <SDL_gamecontroller.h>

struct SDL_Texture;
//struct SDL_GameController;

class Player : public GameObject
{
public:
  Player();
  ~Player();

  void Initialize(SDL_Renderer *renderer);

  void Update(float dt);
  void Draw(SDL_Renderer *renderer, float dt);

  void SetController(SDL_GameController *controller);

protected:
  SDL_Texture *_playerImage;

  float _speed;

  SDL_GameController *_controller;
};