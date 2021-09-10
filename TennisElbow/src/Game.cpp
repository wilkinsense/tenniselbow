#include "Game.h"
#include <SDL.h>
#include <SDL_gamecontroller.h>

#include "TennisScreen.h"

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

Game::Game() : GameEngine() { }

Game::~Game()
{
    SDL_GameControllerClose(_controller);
}

void Game::InitializeImpl()
{
    SDL_SetWindowTitle(_window, "(Not) Another Case of Tennis Elbow");
    SDL_RenderSetScale(_renderer, 4, 4);
    _controller = SDL_GameControllerOpen(0);

    _screenManager = ScreenManager::GetInstance();
    _screenManager->Setup(_window, _renderer);

    TennisScreen* tennis = new TennisScreen(_controller);
    tennis->Initialize(_renderer);

    _screenManager->PushScreen("TENNIS", tennis);
    _screenManager->ShowScreen("TENNIS");
}

void Game::UpdateImpl(float dt)
{
    _screenManager->Update(dt);
}

void Game::DrawImpl(SDL_Renderer* renderer, float dt)
{
    _screenManager->Draw(dt);
}
