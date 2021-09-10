#include "ScreenManager.h"
#include "Screen.h"
#include <SDL.h>

ScreenManager* ScreenManager::_instance = nullptr;

ScreenManager* ScreenManager::GetInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ScreenManager();
    }

    return _instance;
}

void ScreenManager::DestroyInstance()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

ScreenManager::ScreenManager()
{
    _activeScreen = nullptr;
    _isRunning = true;
}

ScreenManager::~ScreenManager()
{
    /*SDL_DestroyTexture(_defaultTarget);

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);

    SDL_Quit();
    */
}

void ScreenManager::Setup(SDL_Window* window, SDL_Renderer* renderer)
{
    _window = window;
    _renderer = renderer;
}

bool ScreenManager::ShowScreen(std::string id)
{
    bool success = false;
    auto screenItr = _screens.find(id);
    if (screenItr != _screens.end())
    {
        if (_activeScreen != nullptr)
        {
            _activeScreen->OnExit();
        }

        //ClearRenderer();

        _activeScreen = screenItr->second;
        _activeScreen->OnEnter();
        success = true;
    }

    return success;
}

bool ScreenManager::PushScreen(std::string id, Screen* screen)
{
    bool success = false;
    if (screen != nullptr)
    {
        auto screenItr = _screens.find(id);
        if (screenItr == _screens.end())
        {
            std::pair<std::string, Screen*> entry(id, screen);
            _screens.insert(entry);
        }
    }

    return success;
}

bool ScreenManager::PopScreen(std::string id)
{
    bool success = false;
    auto screenItr = _screens.find(id);
    if (screenItr != _screens.end())
    {
        if (screenItr->second == _activeScreen)
        {
            _activeScreen->OnExit();
            _activeScreen = nullptr;
        }

        _screens.erase(screenItr);
    }

    return success;
}

void ScreenManager::Update(float dt)
{
    if (_isRunning)
    {
        SDL_Event e;
        SDL_PollEvent(&e);

        if (_activeScreen)
        {
            _activeScreen->Update(e, dt);
        }

        if (e.type == SDL_QUIT)
        {
            _isRunning = false;
        }
    }
}

void ScreenManager::Draw(float dt)
{
    if (_activeScreen)
    {
        //SDL_Texture *oldTarget = SDL_GetRenderTarget(_renderer);
        //SDL_SetRenderTarget(_renderer, _activeScreen->_screenRenderTarget);

        _activeScreen->Draw(_renderer, dt);

        //SDL_SetRenderTarget(_renderer, oldTarget);
        //SDL_RenderCopy(_renderer, _activeScreen->_screenRenderTarget, NULL, NULL);
    }

    //SDL_RenderPresent(_renderer);
    //SDL_UpdateWindowSurface(_window);
}

bool ScreenManager::IsRunning()
{
    return _isRunning;
}

void ScreenManager::SetIsRunning(bool running)
{
    _isRunning = running;
}

void ScreenManager::ClearRenderer()
{
    //SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //SDL_RenderClear(_renderer);

    if (_activeScreen != nullptr)
    {
        _activeScreen->Clear();
    }
}

SDL_Renderer* ScreenManager::GetRenderer() const
{
    return _renderer;
}

SDL_Window* ScreenManager::GetWindow() const
{
    return _window;
}
