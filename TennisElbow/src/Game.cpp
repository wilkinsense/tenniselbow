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
        Vector3 difference = { 0.0f, 0.0f, 0.0f };
        if (_ball.IsOnGround())
        {
          difference = { 0.0f, 0.0f, 5.0f };
        }
        else
        {
          _ballInPlay = true;
          _ball.SetActive(true);
          difference = { 0.0f, -50.0f, 8.5f };
        }
        _ball.ApplyForce(difference);
      }
    }
  }
  else if (_hitNet == false)
  {
    SDL_Rect ballRect, shadowRect;
    _ball.GetDrawRect(&ballRect);
    _ball.GetShadowDrawRect(&shadowRect);

    int netWidth = 136, netHeight = 11;
    SDL_Rect netRect = { _net.GetTransform().position.x - (netWidth / 2), _net.GetTransform().position.y - (netHeight / 2), netWidth, netHeight };

    bool firstIntersect = SDL_HasIntersection(&ballRect, &netRect);
    bool secondIntersect = SDL_HasIntersection(&shadowRect, &netRect);
    float difference = _net.GetTransform().position.y - (_ball.GetTransform().position.y + _ball.GetTransform().position.z);
    bool depthTest = difference < -4.0f;

    if (firstIntersect && secondIntersect && depthTest)
    {
      Vector3 velocity = _ball.GetVelocity();
      velocity.x *= -1.25f;
      velocity.y *= -1.25f;
      velocity.z = 0.0f;

      printf("Depth test: %f\n", difference);

      _ball.ApplyForce(velocity);
      _hitNet = true;
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

  SDL_Rect courtRect = { 0.0f, 0.0f };
  SDL_QueryTexture(_court, nullptr, nullptr, &courtRect.w, &courtRect.h);

  SDL_RenderCopy(_renderer, _court, NULL, &courtRect);

  std::vector<GameObject *> renderOrder;
  CalculateDrawOrder(renderOrder);

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
  {
    (*itr)->Draw(renderer, dt);
  }
}

void Game::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
{
  // SUPER HACK GARBAGE ALGO.
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
  _hitNet = false;

  _player.GetTransform().position = { 100.0f, 125.0f, 0.0f };
  _ball.SetActive(false);

  Vector3 resetVelocity = _ball.GetVelocity();
  resetVelocity = { -resetVelocity.x, -resetVelocity.y, -resetVelocity.z };

  _ball.ApplyForce(resetVelocity);
}