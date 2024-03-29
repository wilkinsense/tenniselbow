#pragma once

#include <Screen.h>
#include <vector>
#include <SDL_gamecontroller.h>

#include "Opponent.h"
#include "Player.h"
#include "Ball.h"
#include "Net.h"

class TennisScreen : public Screen
{
public:
    enum class RoundState
    {
        SERVING,
        PLAY,
        END,
    };

    enum class GameState
    {
        PLAY,
        PAUSED
    };

    TennisScreen(SDL_GameController* controller);
    ~TennisScreen();

    void Update(const SDL_Event& e, float dt);
    void Draw(SDL_Renderer* renderer, float dt);

    void OnEnter() { }
    void OnEnterTransitionDidFinish() { }
    void OnExit() { }
    void OnExitTransitionDidFinish() { }

    void Clear() { }

protected:
    void InitializeImpl(SDL_Renderer* renderer);

    void HandleInput(const SDL_Event* evt);
    void Reset();
    void CalculateDrawOrder(std::vector<GameObject*>& drawOrder);

    bool IsRoundOver();

    Opponent _opponent;
    Player _player;
    Ball _ball;
    Net _net;

    SDL_Texture* _courtImage;

    // Using the default member-wise initializer for our new struct.
    Vector2 _servingDirection;

    RoundState _roundState;
    GameState _gameState;

    bool _hitNet;
    int _bounces;

    Vector3 _hitPosition;
    Vector3 _hitVelocity;

    std::vector<GameObject*> _objects;
    std::vector<Vector3> _impactPoints;

    SDL_GameController* _controller;
};
};
