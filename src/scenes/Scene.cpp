// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "scenes/Scene.h"

#include <SDL.h>

#include <chrono>
#include <fstream>

#include "Game.h"
#include "exceptions/FileSystemException.h"
#include "managers/Input.h"
#include "objects/GameObject.h"
#include "utils/DebugAssert.h"
#include "utils/Time.h"

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
  Input::clear();

  // Handle all events
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) return end();

    Input::update(event);
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
  // Clear the screen
  SDL_RenderClear(Game::renderer());

  for (auto* entry : gameObjects_) entry->onRender();

  // Render the new frame
  SDL_RenderPresent(Game::renderer());
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

  const constexpr static int32_t gameFrameRate = 60;
  const constexpr static double_t frameTime =
      1000.0 / static_cast<double_t>(gameFrameRate);

  size_t frame{0};
  while (!stop_) {
    const auto& start = Time::now();
    onCreate();
    onEvents();
    onUpdate();
    onRender();

    Time::measure(start, frameTime);
    const auto remaining =
        static_cast<int64_t>(frameTime) - Time::elapsedMilliSeconds();

    if (remaining > 0) {
      debug_print(
          "[Frame: %zi] Delaying for %zi milliseconds. Delta time: %f\n",
          frame++, remaining, Time::delta());
      SDL_Delay(static_cast<uint32_t>(remaining));
    } else {
      debug_print("[Frame: %zi] Delta time: %f\n", frame++, Time::delta());
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
