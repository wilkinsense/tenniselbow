#include "Net.h"
#include <SDL.h>
#include <SDL_image.h>

Net::Net() : GameObject()
{
  _transform.position = { 0.0f, 0.0f, 0.0f };
}

Net::~Net()
{

}

void Net::Initialize(SDL_Renderer *renderer)
{
  _netImage = IMG_LoadTexture(renderer, "res/net.png");
}

void Net::Update(float dt)
{

}

void Net::Draw(SDL_Renderer *renderer, float dt)
{
  int width, height;
  SDL_QueryTexture(_netImage, nullptr, nullptr, &width, &height);

  SDL_Rect netRect = { _transform.position.x - (width / 2), _transform.position.y - (height / 2), width, height };
  SDL_RenderCopy(renderer, _netImage, nullptr, &netRect);
}