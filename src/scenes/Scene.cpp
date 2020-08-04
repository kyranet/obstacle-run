// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "Scene.h"

#include <json/json.h>

#include <algorithm>
#include <fstream>

#include "../Game.h"
#include "../exceptions/FileSystemException.h"
#include "../managers/Input.h"
#include "../utils/DebugAssert.h"
#include "../utils/TimePool.h"
#include "SDL.h"

Scene::Scene(const std::string& name) noexcept { name_ = name; }
Scene::~Scene() noexcept { end(); }

void Scene::onStart() noexcept {}

void Scene::onCreate() noexcept {
  size_t last = gameObjects_.size();
  gameObjects_.resize(last + newGameObjects_.size());
  for (auto* object : newGameObjects_) {
    gameObjects_[last++] = object;
    object->onAwake();
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
    if (entry->getDestroy()) {
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

  DEBUG_PRINT("Loading Scene: '%s'\n", root["name"].asCString())
  const auto rawGameObjects = root["game_objects"];
  for (auto object : rawGameObjects) {
    DEBUG_PRINT("Loading GameObject: '%s'.\n", object["name"].asCString())
    auto* gameObject = new GameObject();
    gameObjects_.emplace_back(gameObject);
  }

  DEBUG_PRINT("Successfully loaded Scene '%s' with %zi GameObjects.\n",
              name_.c_str(), gameObjects_.size())

  onStart();
}

void Scene::run() noexcept {
  stop_ = false;

  const static uint32_t gameFrameRate = 60;
  TimePool timePool(1000 / gameFrameRate, SDL_GetTicks());
  while (!stop_) {
    onCreate();
    onEvents();
    onUpdate();
    onRender();

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
  gameObject->setDestroy(true);
}

std::vector<GameObject*> Scene::getGameObjects() const noexcept {
  return gameObjects_;
}

std::string Scene::getName() const noexcept { return name_; }
