#pragma once

#include <GameObject.h>
#include <MathUtils.h>

struct SDL_Texture;
struct SDL_Rect;

class Ball : public GameObject
{
public:
    Ball();
    ~Ball();

    void Initialize(SDL_Renderer* renderer);

    void Update(const SDL_Event& evt, float dt);
    void Draw(SDL_Renderer* renderer, float dt);

    void ApplyForce(Vector3 force);
    void SetActive(bool active);
    bool GetActive();

    bool IsOnGround();
    bool IsBouncing();
    const Vector3& GetVelocity();

    void TEST_SetVelocity(Vector3 velocity);

    void Reset();

    void GetDrawRect(SDL_Rect* drawRect);
    void GetShadowDrawRect(SDL_Rect* shadowDrawRect);

protected:
    SDL_Texture* _ballImage;
    SDL_Texture* _shadowImage;

    Vector3 _velocity;
    float _mass;
    bool _onGround;
    bool _bouncing;
    bool _active;

    int _width, _height;
};