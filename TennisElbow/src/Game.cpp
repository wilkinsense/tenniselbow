#include "Game.h"
#include <SDL.h>
#include <math.h>

// Initializing our static member pointer.
GameEngine* GameEngine::_instance = nullptr;

GameEngine* GameEngine::CreateInstance()
{
  if (_instance == nullptr)
  {
    _instance = new Game();
  }
  return _instance;
}

Game::Game() : GameEngine()
{

}

void Game::InitializeImpl()
{
  // Using the default member-wise initializer for our new struct.
  pos = { 100.0f, 100.0f };
  endPointOffset = { 10.0f, 0.0f };
  speed = 10.0f;
  rotationSpeed = 360.0f;

  _objects.push_back(&_player);
  _objects.push_back(&_ball);

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Initialize(_renderer);
  }

  SDL_RenderSetScale(_renderer, 5, 5);
}

void Game::UpdateImpl(float dt)
{
  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Update(dt);
  }
}

void Game::DrawImpl(SDL_Renderer *renderer, float dt)
{
  // Set the draw colour for our point.
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  // Draw the point.
  //SDL_RenderDrawPoint(renderer, posX, posY);

  static float rotationDegrees = 10.0f;
  rotationDegrees += (rotationSpeed * dt);
  rotationDegrees += (rotationDegrees >= 360.0f ? -360.0f : 0);

  float rotationRadians = MathUtils::ToRadians(rotationDegrees);

  Vector2 rotatedOffset =
  {
    endPointOffset.x * cosf(rotationRadians) + endPointOffset.y * sinf(rotationRadians),
    endPointOffset.x * sinf(rotationRadians) - endPointOffset.y * cosf(rotationRadians)
  };

  Vector2 transformedEndPoint = { pos.x + rotatedOffset.x, pos.y + rotatedOffset.y };

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Draw(renderer, dt);
  }

  SDL_RenderDrawLine(renderer, pos.x, pos.y, transformedEndPoint.x, transformedEndPoint.y);
}