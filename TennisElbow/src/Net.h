#pragma once

#include <GameObject.h>

struct SDL_Texture;

class Net : public GameObject
{
public:
  Net();
  ~Net();

  void Initialize(SDL_Renderer *renderer);

  void Update(float dt);
  void Draw(SDL_Renderer *renderer, float dt);

protected:
  SDL_Texture *_netImage;
};