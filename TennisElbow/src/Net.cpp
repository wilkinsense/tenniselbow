#include "Net.h"
#include <SDL.h>
#include <SDL_image.h>

Net::Net() : GameObject()
{
    _transform.position = { 0.0f, 0.0f, 0.0f };
}

Net::~Net()
{
    SDL_DestroyTexture(_netImage);
}

void Net::Initialize(SDL_Renderer* renderer)
{
    _netImage = IMG_LoadTexture(renderer, "res/net.png");
    SDL_QueryTexture(_netImage, nullptr, nullptr, &_width, &_height);
}

void Net::Update(const SDL_Event& evt, float dt)
{

}

void Net::Draw(SDL_Renderer* renderer, float dt)
{
    SDL_Rect netRect;
    GetDrawRect(&netRect);
    SDL_RenderCopy(renderer, _netImage, nullptr, &netRect);
}

void Net::GetDrawRect(SDL_Rect* drawRect)
{
    (*drawRect) = { (int)_transform.position.x - (_width / 2), (int)_transform.position.y - (_height / 2), _width, _height };
}