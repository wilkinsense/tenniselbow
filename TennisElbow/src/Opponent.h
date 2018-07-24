#pragma once

#include "GameObject.h"

struct SDL_Texture;
class Ball;

class Opponent: public GameObject
{
public:
  Opponent();
  ~Opponent();

  void Initialize(SDL_Renderer *renderer);

  void Update(float dt);
  void Draw(SDL_Renderer *renderer, float dt);

  void CalculateBallImpact(Ball *ball);
  void Reset();

protected:
  SDL_Texture *_opponentImage;
  float _speed;

  Vector3 _direction;

  Vector3 _intendedImpactPoint;
};
