#include "Game.h"
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>

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

Game::~Game()
{
  SDL_DestroyTexture(_court);
}

void Game::InitializeImpl()
{
  _court = IMG_LoadTexture(_renderer, "res/court.png");

  _objects.push_back(&_net);
  _objects.push_back(&_player);
  _objects.push_back(&_ball);

  _net.GetTransform().position = { 69.0f, 71.0f, 0.0f };

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Initialize(_renderer);
  }

  SDL_RenderSetScale(_renderer, 4, 4);
  Reset();
}

void Game::UpdateImpl(float dt)
{
  SDL_Event evt;
  SDL_PollEvent(&evt);

  bool buttonReleased = evt.type == SDL_CONTROLLERBUTTONUP;
  Uint8 button = evt.cbutton.button;

  if (_ballInPlay == false)
  {
    _ball.GetTransform().position.x = _player.GetTransform().position.x + 5;
    _ball.GetTransform().position.y = _player.GetTransform().position.y + 5;
    if (buttonReleased)
    {
      if (evt.cbutton.button == SDL_CONTROLLER_BUTTON_A)
      {
        if (_ball.IsOnGround())
        {
          Vector3 difference = { 0.0f,//_ball.GetTransform().position.x - _player.GetTransform().position.x,
            0.0f,//_ball.GetTransform().position.y - _player.GetTransform().position.y,
            5.0f };
          _ball.ApplyForce(difference);
        }
        else
        {
          _ballInPlay = true;
          _ball.SetActive(true);
          Vector3 difference = { 0.0f,
            -50.0f,
            8.5f };
          _ball.ApplyForce(difference);
        }
      }
    }
  }

  if (buttonReleased && button == SDL_CONTROLLER_BUTTON_X)
  {
    Reset();
  }

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

  SDL_Rect courtRect = { 0.0f, 0.0f, 154.0f, 138.0f };
  SDL_QueryTexture(_court, nullptr, nullptr, &courtRect.w, &courtRect.h);

  SDL_RenderCopy(_renderer, _court, NULL, &courtRect);

  std::vector<GameObject *> renderOrder;
  CalculateDrawOrder(renderOrder);

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
  {
    (*itr)->Draw(renderer, dt);
  }

  SDL_RenderDrawLine(renderer, pos.x, pos.y, transformedEndPoint.x, transformedEndPoint.y);
}

void Game::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
{
  drawOrder.clear();

  auto objectsCopy = _objects;
  auto farthestEntry = objectsCopy.begin();
  while (objectsCopy.size() > 0)
  {
    bool entryFound = true;
    for (auto itr = objectsCopy.begin(); itr != objectsCopy.end(); itr++)
    {
      if (farthestEntry != itr)
      {
        if ((*itr)->GetTransform().position.y < (*farthestEntry)->GetTransform().position.y)
        {
          entryFound = false;
          farthestEntry = itr;
          break;
        }
      }
    }

    if (entryFound)
    {
      GameObject *farthest = *farthestEntry;

      drawOrder.push_back(farthest);
      objectsCopy.erase(farthestEntry);
      farthestEntry = objectsCopy.begin();
    }
  }
}

void Game::Reset()
{
  // Using the default member-wise initializer for our new struct.
  pos = { 100.0f, 100.0f };
  endPointOffset = { 10.0f, 0.0f };
  speed = 10.0f;
  rotationSpeed = 360.0f;

  _ballInPlay = false;

  _player.GetTransform().position = { 100.0f, 125.0f, 0.0f };
  _ball.SetActive(false);

  Vector3 resetVelocity = _ball.GetVelocity();
  resetVelocity = { -resetVelocity.x, -resetVelocity.y, -resetVelocity.z };

  _ball.ApplyForce(resetVelocity);
}