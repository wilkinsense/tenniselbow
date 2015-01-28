#include "Game.h"
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_gamecontroller.h>

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
  SDL_DestroyTexture(_courtImage);
  SDL_GameControllerClose(_controller);
}

void Game::InitializeImpl()
{
  SDL_SetWindowTitle(_window, "(Not) Another Case of Tennis Elbow");
  _courtImage = IMG_LoadTexture(_renderer, "res/court.png");

  _objects.push_back(&_net);
  _objects.push_back(&_player);
  _objects.push_back(&_ball);

  _controller = SDL_GameControllerOpen(0);
  _player.SetController(_controller);

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

  HandleInput(&evt);

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
  SDL_QueryTexture(_courtImage, nullptr, nullptr, &courtRect.w, &courtRect.h);

  SDL_RenderCopy(_renderer, _courtImage, NULL, &courtRect);

  std::vector<GameObject *> renderOrder;
  CalculateDrawOrder(renderOrder);

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
  {
    (*itr)->Draw(renderer, dt);
  }

  if (_state == GAME_STATE_SERVING)
  {
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, SDL_ALPHA_OPAQUE);

    SDL_RenderDrawLine(renderer, _player.GetTransform().position.x, _player.GetTransform().position.y,
      _player.GetTransform().position.x + _servingDirection.x, _player.GetTransform().position.y + _servingDirection.y);

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
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
  _servingDirection = { 0.0f, 0.0f };

  _state = GAME_STATE_SERVING;

  _player.GetTransform().position = { 100.0f, 125.0f, 0.0f };
  _ball.SetActive(false);

  Vector3 resetVelocity = _ball.GetVelocity();
  resetVelocity = { -resetVelocity.x, -resetVelocity.y, -resetVelocity.z };

  _ball.ApplyForce(resetVelocity);
}

void Game::HandleInput(SDL_Event *evt)
{
  bool buttonReleased = evt->type == SDL_CONTROLLERBUTTONUP;
  Uint8 button = evt->cbutton.button;
  
  if (_state == GAME_STATE_SERVING)
  {
    _servingDirection = { (70 - _player.GetTransform().position.x) * 2,
      (15 - _player.GetTransform().position.y) };

    _ball.GetTransform().position.x = _player.GetTransform().position.x + 5;
    _ball.GetTransform().position.y = _player.GetTransform().position.y + 5;
    if (buttonReleased)
    {
      if (evt->cbutton.button == SDL_CONTROLLER_BUTTON_A)
      {
        Vector3 difference = { 0.0f, 0.0f, 0.0f };
        if (_ball.IsOnGround())
        {
          difference = { 0.0f, 0.0f, 5.0f };
        }
        else
        {
          _state = GAME_STATE_PLAY;
          _ball.SetActive(true);
          float velocityZ = (_ball.GetVelocity().z < 0.0f) ? 7.5f : 3.0f;

          float hyp = sqrtf(powf(_servingDirection.x, 2) + powf(_servingDirection.y, 2));
          float magX = _servingDirection.x / hyp;
          float magY = _servingDirection.y / hyp;
          difference = { 50.0f * magX, 50.0f * magY, velocityZ };
        }
        _ball.ApplyForce(difference);
      }
    }
  }
  else if (_state == GAME_STATE_PLAY)
  {
    SDL_Rect ballRect, shadowRect;
    _ball.GetDrawRect(&ballRect);
    _ball.GetShadowDrawRect(&shadowRect);

    SDL_Rect netRect;
    _net.GetDrawRect(&netRect);

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
      _state = GAME_STATE_END_ROUND;
    }
  }

  if (buttonReleased && button == SDL_CONTROLLER_BUTTON_X)
  {
    Reset();
  }
}