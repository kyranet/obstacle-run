#include "Game.h"

#include <iostream>

#include "SDL_mixer.h"
#include "SDL_net.h"
#include "SDL_ttf.h"

Game::Game() = default;
Game::~Game() { end(); }

Game* Game::instance_ = nullptr;

bool Game::start() {
  if (!init()) return false;

  window_ = SDL_CreateWindow("Obstacle Run", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    std::cerr << "Could not create a window.";
    return false;
  }

  renderer_ = SDL_CreateRenderer(
      window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer_ == nullptr) {
    std::cerr << "Could not create a renderer.";
    return false;
  }

  surface_ = SDL_GetWindowSurface(window_);

  // Fill the surface white
  SDL_FillRect(surface_, nullptr,
               SDL_MapRGB(surface_->format, 0xFF, 0xFF, 0xFF));

  // Update the surface
  SDL_UpdateWindowSurface(window_);

  // Wait two seconds
  SDL_Delay(2000);

  return true;
}

bool Game::end() {
  // Destroy window
  SDL_DestroyWindow(window_);

  // Quit SDL subsystems
  SDL_Quit();
  TTF_Quit();
  Mix_Quit();
  SDLNet_Quit();

  return true;
}

bool Game::init() {
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
    std::cerr << "Error initializing SDL.\nReason: " << SDL_GetError();
    return false;
  }

  if (TTF_Init() != 0) {
    std::cerr << "Error initializing TTF.\nReason: " << TTF_GetError();
    return false;
  }

  if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
    std::cerr << "Error initializing MIX.\nReason: " << Mix_GetError();
    return false;
  }

  if (SDLNet_Init() != 0) {
    std::cerr << "Error initializing NET.\nReason: " << SDLNet_GetError();
    return false;
  }

  return true;
}

Game* Game::getInstance() {
  if (instance_ == nullptr) {
    instance_ = new Game();
  }

  return instance_;
}
