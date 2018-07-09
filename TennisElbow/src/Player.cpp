#include "Player.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#define DEADZONE 0.25f

Player::Player() : GameObject()
{
  _speed = 50.0f;
}

Player::~Player()
{
  SDL_DestroyTexture(_playerImage);
  _controller = nullptr;
}

void Player::Initialize(SDL_Renderer *renderer)
{
  _playerImage = IMG_LoadTexture(renderer, "res/me.png");
}

void Player::Update(float dt)
{
  float xvalue = (float)SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_LEFTX) / (float)SHRT_MAX;
  float yvalue = (float)SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_LEFTY) / (float)SHRT_MAX;

  if (abs(xvalue) < DEADZONE)
  {
    xvalue = 0.0f;
  }

  if (abs(yvalue) < DEADZONE)
  {
    yvalue = 0.0f;
  }

  _transform.position.x += xvalue * _speed * dt;
  _transform.position.y += yvalue * _speed * dt;
}

void Player::Draw(SDL_Renderer *renderer, float dt)
{
  int width, height;
  SDL_QueryTexture(_playerImage, nullptr, nullptr, &width, &height);

  SDL_Rect location = { _transform.position.x - (width / 2), _transform.position.y - (height / 2), width, height };
  SDL_RenderCopy(renderer, _playerImage, nullptr, &location);
}

void Player::SetController(SDL_GameController *controller)
{
  _controller = controller;
}