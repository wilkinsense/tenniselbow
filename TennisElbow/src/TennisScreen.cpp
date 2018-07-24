#include "TennisScreen.h"
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>

#include <MathConstants.h>
#include "GameConstants.h"

TennisScreen::TennisScreen(SDL_GameController *controller): 
  _controller(controller) { }

TennisScreen::~TennisScreen()
{
  SDL_DestroyTexture(_courtImage);
}

void TennisScreen::InitializeImpl(SDL_Renderer *renderer)
{
  _courtImage = IMG_LoadTexture(renderer, "res/court.png");

  _objects.push_back(&_net);
  _objects.push_back(&_ball);
  _objects.push_back(&_player);
  _objects.push_back(&_opponent);

  _player.SetController(_controller);

  _net.GetTransform().position = { 69.0f, 71.0f, 0.0f };

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Initialize(renderer);
  }

  SDL_RenderSetScale(renderer, 4, 4);
  Reset();

  // Let Replay serve values
  /*_hitPosition = { 105.0f, 130.0f, 0.745036876f };
  _hitVelocity = { 0.0f, 0.0f, -3.49657798f };*/

  _hitPosition = { 105.0f, 130.0f, 1.27140641f };
  _hitVelocity = { 0.0f, 0.0f, 0.0608121306f };
}

void TennisScreen::Reset()
{
  // Using the default member-wise initializer for our new struct.
  _servingDirection = { 0.0f, 0.0f };

  _roundState = ROUND_STATE_SERVING;
  _gameState = GAME_STATE_PLAY;

  _hitNet = false;
  _bounces = 0;

  // Default positions for player and opponent; this will have to change once rounds are implemented.
  _player.GetTransform().position = { 100.0f, 125.0f, 0.0f };
  _opponent.GetTransform().position = { 30.0f, 15.0f, 0.0f };

  _ball.Reset();
  _opponent.Reset();
  _impactPoints.clear();
}

void TennisScreen::Update(const SDL_Event &e, float dt) {
  HandleInput(&e);

  float timescale = 1.0f;
  dt *= timescale;

  if (dt > 0.33f)
  {
    dt = 0.33f;
  }

  if (_gameState == GAME_STATE_PLAY)
  {
    for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
    {
      (*itr)->Update(dt);
    }

    if (_ball.IsBouncing())
    {
      _impactPoints.push_back(_ball.GetTransform().position);
    }

    if (_roundState == ROUND_STATE_PLAY)
    {
      if (_ball.IsBouncing())
      {
        _bounces++;
        _opponent.CalculateBallImpact(&_ball);
      }

      if (IsRoundOver())
      {
        _roundState = ROUND_STATE_END;
        _opponent.Reset();
      }
    }
  }
}

void TennisScreen::HandleInput(const SDL_Event *evt)
{
  bool buttonReleased = evt->type == SDL_CONTROLLERBUTTONUP;
  Uint8 button = evt->cbutton.button;

  if (_roundState == ROUND_STATE_SERVING)
  {
    _servingDirection = { (SERVING_DIRECTION_OFFSET_X - _player.GetTransform().position.x) * 2,
      (SERVING_DIRECTION_OFFSET_Y - _player.GetTransform().position.y) };

    _ball.GetTransform().position.x = _player.GetTransform().position.x + BALL_SERVING_OFFSET_X;
    _ball.GetTransform().position.y = _player.GetTransform().position.y + BALL_SERVING_OFFSET_Y;
    if (buttonReleased)
    {
      bool replay = evt->cbutton.button == SDL_CONTROLLER_BUTTON_B;
      if (replay)
      {
        _ball.Reset();
        _ball.TEST_SetVelocity(_hitVelocity);
        _ball.GetTransform().position = _hitPosition;
      }

      if (evt->cbutton.button == SDL_CONTROLLER_BUTTON_A || replay)
      {
        Vector3 difference = { 0.0f, 0.0f, 0.0f };
        if (_ball.IsOnGround())
        {
          difference = { 0.0f, 0.0f, BALL_THROW_HEIGHT };
        }
        else
        {
          _roundState = ROUND_STATE_PLAY;
          _ball.SetActive(true);
          float velocityZ = -PHYSICS_GRAVITY * ((_ball.GetVelocity().z < 0.0f) ? 0.75f : 0.33f);

          float hyp = sqrtf(powf(_servingDirection.x, 2) + powf(_servingDirection.y, 2));
          float magX = _servingDirection.x / hyp;
          float magY = _servingDirection.y / hyp;
          Vector2 normalizedServingDirection = MathUtils::Normalize(_servingDirection);
          difference = { SERVE_STRENGTH * normalizedServingDirection.x,
            SERVE_STRENGTH * normalizedServingDirection.y,
            velocityZ };

          _hitPosition = _ball.GetTransform().position;
          _hitVelocity = _ball.GetVelocity();
        }
        _ball.ApplyForce(difference);

        if (_roundState == ROUND_STATE_PLAY)
        {
          _opponent.CalculateBallImpact(&_ball);
        }
      }
    }
  }
  else if (_roundState == ROUND_STATE_PLAY)
  {
    SDL_Rect ballRect, shadowRect;
    _ball.GetDrawRect(&ballRect);
    _ball.GetShadowDrawRect(&shadowRect);

    SDL_Rect netRect;
    _net.GetDrawRect(&netRect);

    Vector3 velocity = _ball.GetVelocity();
    Vector3 normalizedVelocity = { velocity.x, velocity.y, velocity.z * 15.0f };
    normalizedVelocity = MathUtils::Normalize(normalizedVelocity);

    bool ballIntersection = SDL_HasIntersection(&ballRect, &netRect);
    bool shadowIntersection = SDL_HasIntersection(&shadowRect, &netRect);
    float difference = (_net.GetTransform().position.y + (netRect.h / 2.0f)) - (_ball.GetTransform().position.y - (_ball.GetTransform().position.z * 15));
    float correctedDifference = difference + normalizedVelocity.z;

    bool hitNetTest = correctedDifference <= NET_HEIGHT_THRESHOLD;
    bool letTest = correctedDifference < LET_ZONE_THRESHOLD;

    if (ballIntersection && shadowIntersection && !_hitNet)
    {
      _hitNet = hitNetTest || letTest;

      if (hitNetTest)
      {
        velocity.x *= NET_HIT_BOUNCE_DECAY;
        velocity.y *= NET_HIT_BOUNCE_DECAY;
        velocity.z = 0.0f;
        //_roundState = ROUND_STATE_END;

        _ball.ApplyForce(velocity);
      }
      else if (letTest)
      {
        velocity.x *= LET_HIT_BOUNCE_DECAY_XY;
        velocity.y *= LET_HIT_BOUNCE_DECAY_XY;
        velocity.z *= LET_HIT_BOUNCE_DECAY_Z;

        _ball.ApplyForce(velocity);
        _impactPoints.push_back(_ball.GetTransform().position);
      }
    }
  }

  if (buttonReleased && button == SDL_CONTROLLER_BUTTON_X)
  {
    Reset();
  }
}

void TennisScreen::Draw(SDL_Renderer *renderer, float dt)
{
  // Set the draw colour for our point.
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  SDL_Rect courtRect = { 0, 0 };
  SDL_QueryTexture(_courtImage, nullptr, nullptr, &courtRect.w, &courtRect.h);

  SDL_RenderCopy(renderer, _courtImage, NULL, &courtRect);

  std::vector<GameObject *> renderOrder;
  CalculateDrawOrder(renderOrder);

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
  {
    (*itr)->Draw(renderer, dt);
  }

  if (_roundState == ROUND_STATE_SERVING)
  {
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, SDL_ALPHA_OPAQUE);

    SDL_RenderDrawLine(renderer, (int)(_player.GetTransform().position.x), (int)(_player.GetTransform().position.y),
      (int)(_player.GetTransform().position.x + _servingDirection.x), (int)(_player.GetTransform().position.y + _servingDirection.y));

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
  }
  else if (!_impactPoints.empty())
  {
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, SDL_ALPHA_OPAQUE);

    for (int pointIndex = 0; pointIndex < _impactPoints.size(); pointIndex++) {
      Vector3 impactPoint = _impactPoints[pointIndex];

      SDL_Rect impactLocation = { (int)impactPoint.x - (5), (int)impactPoint.y - (5), 10, 10 };
      SDL_RenderDrawRect(renderer, &impactLocation);
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
  }
}

void TennisScreen::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
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

bool TennisScreen::IsRoundOver()
{
  bool multipleBounces = _bounces >= 2;
  bool ballStopped = (fabsf(_ball.GetVelocity().x) <= BALL_STOPPED_EPSILON &&
    fabsf(_ball.GetVelocity().y) <= BALL_STOPPED_EPSILON &&
    fabsf(_ball.GetVelocity().z) <= BALL_STOPPED_EPSILON &&
    _ball.IsOnGround() == true);
  bool result = _roundState == ROUND_STATE_PLAY &&
    (multipleBounces || _hitNet || ballStopped );
  return result;
}
