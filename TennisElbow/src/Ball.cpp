#include "Ball.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <iostream>

Ball::Ball() : GameObject()
{
  _transform.position = { 0.0f, 0.0f, 0.0f };
  _mass = 0.146f;

  _velocity = { 0.0f, 0.0f, 0.0f };
  _bouncing = false;
  _onGround = false;
  _active = false;
}

Ball::~Ball()
{
  SDL_DestroyTexture(_ballImage);
  SDL_DestroyTexture(_shadowImage);
}

void Ball::Initialize(SDL_Renderer *renderer)
{
  _ballImage = IMG_LoadTexture(renderer, "res/ball.png");
  _shadowImage = IMG_LoadTexture(renderer, "res/shadow.png");
}

void Ball::Update(float dt)
{
  if (_bouncing == true)
  {
    _onGround = false;
    _bouncing = false;
  }

  if (_onGround == false)
  {
    _velocity.z += (-9.8f * dt);
    _transform.position.z += _velocity.z * dt;
  }

  _transform.position.x += _velocity.x * dt;
  _transform.position.y += _velocity.y * dt;

  if (_transform.position.z <= 0.0f)
  {
    _velocity.z *= -0.5f;
    _transform.position.z = 0.0f;
    _onGround = true;

    if (_velocity.z != 0.0f && _active)
    {
      _bouncing = true;
      _velocity.x *= 0.75f;
      _velocity.y *= 0.75f;
    }
    else if (!_active)
    {
      _velocity.x = 0.0f;
      _velocity.y = 0.0f;
    }
  }

  if (_onGround)
  {
    if (_velocity.x != 0.0f)
    {
      _velocity.x += -(_velocity.x / fabsf(_velocity.x)) * 0.025f;
    }
    
    if (_velocity.y != 0.0f)
    {
      _velocity.y += -(_velocity.y / fabsf(_velocity.y)) * 0.025f;
    }
  }
}

void Ball::Draw(SDL_Renderer *renderer, float dt)
{ 
  SDL_Rect location = { _transform.position.x, _transform.position.y, 4, 4 };
  SDL_RenderCopy(renderer, _shadowImage, nullptr, &location);

  location = { _transform.position.x, _transform.position.y - (_transform.position.z * 15.0f), 4, 4 };
  SDL_RenderCopy(renderer, _ballImage, nullptr, &location);
}

void Ball::ApplyForce(Vector3 force)
{ 
  _velocity.x += force.x;
  _velocity.y += force.y;
  _velocity.z += force.z;

  if (_velocity.z != 0.0f)
  {
    _onGround = false;
  }
}

const Vector3& Ball::GetVelocity()
{
  return _velocity;
}

void Ball::SetActive(bool active)
{
  _active = active;
}

bool Ball::GetActive()
{
  return _active;
}

bool Ball::IsOnGround()
{
  return _onGround;
}

bool Ball::IsBouncing()
{
  return _bouncing;
}