// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "scenes/Scene.h"

#include <algorithm>
#include <fstream>

#include "Game.h"
#include "SDL.h"
#include "exceptions/FileSystemException.h"
#include "managers/Input.h"
#include "utils/DebugAssert.h"
#include "utils/TimePool.h"

Scene::Scene(const std::string& name) noexcept { name_ = name; }
Scene::~Scene() noexcept { end(); }

void Scene::onStart() const noexcept {
  for (const auto& gameObject : gameObjects()) {
    gameObject->onAwake();
  }
}

void Scene::onCreate() noexcept {
  size_t last = gameObjects_.size();
  gameObjects_.resize(last + newGameObjects_.size());
  for (auto* gameObject : newGameObjects_) {
    gameObjects_[last++] = gameObject;
    gameObject->onAwake();
  }

  newGameObjects_.clear();
}

void Scene::onEvents() noexcept {
  // Clear the Input's cache
  Input::instance()->clear();

  // Handle all events
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) return end();

    Input::instance()->update(event);
  }
}

void Scene::onUpdate() noexcept {
  size_t i = 0;
  while (i < gameObjects_.size()) {
    auto* entry = gameObjects_[i];
    entry->onUpdate();
    if (entry->destroy()) {
      gameObjects_.erase(gameObjects_.begin() + i);
      entry->onDestroy();
      delete entry;
    } else {
      ++i;
    }
  }
}

void Scene::onRender() noexcept {
  for (auto* entry : gameObjects_) entry->onRender();
}

void Scene::onEnd() noexcept {
  for (auto* entry : newGameObjects_) delete entry;
  newGameObjects_.clear();

  for (auto* entry : gameObjects_) delete entry;
  gameObjects_.clear();
}

void Scene::load() {
  std::string path = "./assets/scenes/" + name_ + ".json";
  Json::Value root;
  Json::CharReaderBuilder builder;
  std::ifstream stream(path, std::ifstream::binary);
  std::string errors;
  if (!Json::parseFromStream(builder, stream, &root, &errors)) {
    throw FileSystemException("Could not parse JSON body from '" + path +
                              "'. Reason: " + errors);
  }

  debug_print("Loading Scene: '%s'.\n", root["name"].asCString());
  const auto rawGameObjects = root["game_objects"];
  for (const auto& object : rawGameObjects) {
    debug_print("Loading GameObject: '%s'.\n", object["name"].asCString());
    auto* gameObject = new GameObject();
    gameObjects_.emplace_back(gameObject);
    gameObject->load(object);
  }

  debug_print("Successfully loaded Scene '%s' with %zi GameObject(s).\n",
              name_.c_str(), gameObjects_.size());

  onStart();
}

void Scene::run() noexcept {
  stop_ = false;

  const static uint32_t gameFrameRate = 60;
  TimePool timePool(1000 / gameFrameRate, SDL_GetTicks());
  while (!stop_) {
    // Clear the screen
    SDL_RenderClear(Game::renderer());

    onCreate();
    onEvents();
    onUpdate();
    onRender();

    // Render the new frame
    SDL_RenderPresent(Game::renderer());

    if (timePool.next(SDL_GetTicks())) {
      SDL_Delay(timePool.getRemaining());
    }
  }
}

void Scene::end() noexcept {
  stop();
  Game::getInstance()->end();
}

void Scene::stop() noexcept {
  stop_ = true;
  onEnd();
}

void Scene::addGameObject(GameObject* gameObject) noexcept {
  gameObjects_.emplace_back(gameObject);
}

void Scene::removeGameObject(GameObject* gameObject) noexcept {
  gameObject->destroy() = true;
}
