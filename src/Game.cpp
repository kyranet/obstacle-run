// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "Game.h"

#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#include <iostream>

#include "factories/ButtonFactory.h"
#include "factories/ImageRendererFactory.h"
#include "factories/PlayerControllerFactory.h"
#include "factories/SolidRendererFactory.h"
#include "factories/TextRendererFactory.h"
#include "factories/TransformFactory.h"
#include "managers/ComponentManager.h"
#include "managers/FontManager.h"
#include "managers/ImageManager.h"
#include "managers/SceneManager.h"
#include "scenes/Scene.h"

Game::Game() noexcept = default;
Game::~Game() noexcept { end(); }

Game* Game::instance_ = nullptr;
SDL_Window* Game::window_ = nullptr;
SDL_Renderer* Game::renderer_ = nullptr;

bool Game::start() noexcept {
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

  return true;
}

bool Game::end() noexcept {
  // Destroy window
  SDL_DestroyWindow(window_);

  // Quit SDL subsystems
  SDL_Quit();
  TTF_Quit();
  Mix_Quit();
  SDLNet_Quit();

  return true;
}

bool Game::init() noexcept {
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

  ComponentManager::getInstance()
      ->add(new TransformFactory())
      ->add(new ImageRendererFactory())
      ->add(new PlayerControllerFactory())
      ->add(new SolidRendererFactory())
      ->add(new ButtonFactory())
      ->add(new TextRendererFactory());

  return true;
}

void Game::run() {
  ImageManager::instance()->init();
  FontManager::instance()->init();

  auto scene = SceneManager::createScene("menu");
  SceneManager::loadScene(scene);
  SceneManager::setActiveScene(scene);

  scene->run();
}

Game* Game::getInstance() {
  if (instance_ == nullptr) {
    instance_ = new Game();
  }

  return instance_;
}
