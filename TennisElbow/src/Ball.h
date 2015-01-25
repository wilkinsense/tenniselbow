#pragma once

#include <GameObject.h>
#include <MathUtils.h>

struct SDL_Texture;

class Ball : public GameObject
{
public:
  Ball();
  ~Ball();

  void Initialize(SDL_Renderer *renderer);

  void Update(float dt);
  void Draw(SDL_Renderer *renderer, float dt);

protected:
  SDL_Texture *_ballImage;
  SDL_Texture *_shadowImage;

  Vector3 _velocity;
  float _mass;
  const float timeScale = 4.0f;
  bool _onGround;
  bool _bouncing;
};