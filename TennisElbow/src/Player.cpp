#include "Player.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

Player::Player() : GameObject()
{
  _transform.position = { 10.0f, 10.0f, 0.0f };
  _speed = 50.0f;
}

Player::~Player()
{
  SDL_DestroyTexture(_playerImage);
}

void Player::Initialize(SDL_Renderer *renderer)
{
  _playerImage = IMG_LoadTexture(renderer, "res/me.png");
  _controller = SDL_GameControllerOpen(0);
}

void Player::Update(float dt)
{
  SDL_Event evt;
  SDL_PollEvent(&evt);
  {
    SDL_ControllerAxisEvent &axisEvt = evt.caxis;
    float value = (float)axisEvt.value / (float)SHRT_MAX;

    float xvalue = (float)SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_LEFTX) / (float)SHRT_MAX;
    float yvalue = (float)SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_LEFTY) / (float)SHRT_MAX;

    _transform.position.x += xvalue * _speed * dt;
    _transform.position.y += yvalue * _speed * dt;
  }
}

void Player::Draw(SDL_Renderer *renderer, float dt)
{
  SDL_RenderDrawLine(renderer,
    _transform.position.x,
    _transform.position.y,
    _transform.position.x + 10,
    _transform.position.y + 10);
  
  SDL_Rect location = { _transform.position.x, _transform.position.y, 16, 16 };
  SDL_RenderCopy(renderer, _playerImage, nullptr, &location);
}