#pragma once

#include "GameObject.h"
#include <SDL_gamecontroller.h>

struct SDL_Texture;

class Player : public GameObject
{
public:
    Player();
    ~Player();

    void Initialize(SDL_Renderer* renderer);

    void Update(const SDL_Event& evt, float dt);
    void Draw(SDL_Renderer* renderer, float dt);

    void SetController(SDL_GameController* controller);

protected:
    SDL_Texture* _playerImage = nullptr;
    SDL_GameController* _controller = __nullptr;

    float _speed;
};