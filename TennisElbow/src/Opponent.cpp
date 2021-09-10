#include "Opponent.h"
#include <SDL.h>
#include <SDL_image.h>
#include <MathConstants.h>
#include "GameConstants.h"
#include "Ball.h"
#include <stdio.h>

Opponent::Opponent() : GameObject()
{
    _speed = PLAYER_SPEED;
    Reset();
}

Opponent::~Opponent()
{
    SDL_DestroyTexture(_opponentImage);
}

void Opponent::Initialize(SDL_Renderer* renderer)
{
    _opponentImage = IMG_LoadTexture(renderer, "res/me.png");
}

void Opponent::Update(const SDL_Event& evt, float dt)
{
    if (_intendedImpactPoint.x != -1.0f)
    {
        _direction = _intendedImpactPoint - GetTransform().position;
        if (MathUtils::Magnitude(_direction) > 1.0f) {
            _direction = MathUtils::Normalize(_direction);
        }
    }
    else
    {
        _direction = Vector3::Zero;
    }

    _transform.position.x += _direction.x * _speed * dt;
    _transform.position.y += _direction.y * _speed * dt;
}

void Opponent::Draw(SDL_Renderer* renderer, float dt)
{
    int width, height;
    SDL_QueryTexture(_opponentImage, nullptr, nullptr, &width, &height);

    SDL_Rect location = { (int)_transform.position.x - (width / 2), (int)_transform.position.y - (height / 2), width, height };
    SDL_RenderCopy(renderer, _opponentImage, nullptr, &location);

    if (_intendedImpactPoint.x != -1.0f)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

        SDL_Rect impactLocation = { (int)_intendedImpactPoint.x - (5), (int)_intendedImpactPoint.y - (5), 10, 10 };
        SDL_RenderDrawRect(renderer, &impactLocation);

        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    }
}

void Opponent::Reset()
{
    _direction = Vector3::Zero;
    _intendedImpactPoint = { -1.0f, -1.0f, 0.0f };
}

void Opponent::CalculateBallImpact(Ball* ball)
{
    Vector3 currentVelocity = ball->GetVelocity();
    Vector3 currentPosition = ball->GetTransform().position;

    float airtime = (fabsf((currentPosition.z + currentVelocity.z) / (Z_HEIGHT_CORRECTION + PHYSICS_GRAVITY)) / ACCURACY_CORRECTION);

    _intendedImpactPoint = {
      currentPosition.x + (currentVelocity.x * airtime),
      currentPosition.y + (currentVelocity.y * airtime),
      0.0f
    };
}