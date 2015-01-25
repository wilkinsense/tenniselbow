#pragma once

#include <GameObject.h>

struct SDL_Texture;
struct SDL_Rect;

class Net : public GameObject
{
public:
  Net();
  ~Net();

  void Initialize(SDL_Renderer *renderer);

  void Update(float dt);
  void Draw(SDL_Renderer *renderer, float dt);

  void GetDrawRect(SDL_Rect *drawRect);

protected:
  SDL_Texture *_netImage;
  int _width, _height;
};