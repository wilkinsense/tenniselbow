#pragma once 

#include "MathUtils.h"
#include <SDL_events.h>

struct SDL_Renderer;

class GameObject
{
public:
    ~GameObject();

    virtual void Initialize(SDL_Renderer* renderer) = 0;

    virtual void Update(const SDL_Event& evt, float dt) = 0;
    virtual void Draw(SDL_Renderer* renderer, float dt) = 0;

    Transform& GetTransform();

protected:
    GameObject();

    Transform _transform;
};