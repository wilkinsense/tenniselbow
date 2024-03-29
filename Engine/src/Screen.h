#include <SDL_events.h>
struct SDL_Renderer;
struct SDL_Texture;
class ScreenManager;

class Screen
{
public:
    friend class ScreenManager;

    Screen() { }
    virtual ~Screen();

    void Initialize(SDL_Renderer* renderer);
    virtual void Update(const SDL_Event& e, float dt) = 0;
    virtual void Draw(SDL_Renderer* renderer, float dt) = 0;

    virtual void OnEnter() { }
    virtual void OnEnterTransitionDidFinish() { }
    virtual void OnExit() { }
    virtual void OnExitTransitionDidFinish() { }

    virtual void Clear() { }

protected:
    virtual void InitializeImpl(SDL_Renderer* renderer) = 0;

    //SDL_Texture* _screenRenderTarget;
};