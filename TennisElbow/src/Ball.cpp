#include "Ball.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include <iostream>

#define BOUNCE_VELOCITY_DECAY 0.75f
#define GROUND_FRICTION 0.0125f
#define GRAVITY -9.8f

Ball::Ball() : GameObject()
{
  Reset();

  _mass = 0.146f;

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

  SDL_QueryTexture(_ballImage, nullptr, nullptr, &_width, &_height);
}

void Ball::Reset()
{
  _transform.position = { 0.0f, 0.0f, 0.0f };

  _velocity = { 0.0f, 0.0f, 0.0f };
  _bouncing = false;
  _onGround = false;
  _active = false;
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
    _velocity.z += (GRAVITY * dt);
    _transform.position.z += _velocity.z * dt;
  }

  _transform.position.x += _velocity.x * dt;
  _transform.position.y += _velocity.y * dt;

  if (_transform.position.z <= 0.0f)
  {
    // Now that we've made impact with the ground, half the velocity and send it back the way it came.
    _velocity.z *= -0.5f;

    // Make sure that the ball didn't go through the ground.
    _transform.position.z = 0.0f;
    _onGround = true;

    // If we're still active, and we've got some velocity left, then we're bouncing.
    if (_velocity.z != 0.0f && _active)
    {
      _bouncing = true;
      _velocity.x *= BOUNCE_VELOCITY_DECAY;
      _velocity.y *= BOUNCE_VELOCITY_DECAY;
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
      _velocity.x += -(_velocity.x / fabsf(_velocity.x)) * GROUND_FRICTION;
    }
    
    if (_velocity.y != 0.0f)
    {
      _velocity.y += -(_velocity.y / fabsf(_velocity.y)) * GROUND_FRICTION;
    }
  }
}

void Ball::Draw(SDL_Renderer *renderer, float dt)
{ 
  SDL_Rect location;
  GetShadowDrawRect(&location);
  SDL_RenderCopy(renderer, _shadowImage, nullptr, &location);

  GetDrawRect(&location);
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

void Ball::GetShadowDrawRect(SDL_Rect *shadowDrawRect)
{
  (*shadowDrawRect) = { (int)_transform.position.x - (_width / 2), (int)_transform.position.y - (_height / 2), _width, _height };
}

void Ball::GetDrawRect(SDL_Rect *drawRect)
{
  (*drawRect) = { (int)_transform.position.x - (_width / 2), (int)_transform.position.y - (int)(_transform.position.z * 15.0f) - (_height / 2), _width, _height };
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