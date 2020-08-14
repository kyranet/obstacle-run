// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "scenes/Scene.h"

#include <SDL.h>

#include <fstream>
#include <utility>

#include "Game.h"
#include "exceptions/FileSystemException.h"
#include "managers/Input.h"
#include "utils/DebugAssert.h"
#include "utils/Time.h"

Scene::Scene(std::string name) noexcept
    : name_(std::move(name)), world_({0.f, 0.f}), contactListener_() {}

Scene::~Scene() noexcept { end(); }

void Scene::onStart() const noexcept {
  for (const auto& gameObject : gameObjects()) {
    if (gameObject->active()) gameObject->onAwake();
  }
}

void Scene::onCreate() noexcept {
  size_t last = gameObjects_.size();
  gameObjects_.resize(last + newGameObjects_.size());
  for (auto& gameObject : newGameObjects_) {
    if (gameObject->active()) gameObject->onAwake();

    std::shared_ptr<GameObject> sharedPtr(std::move(gameObject));
    gameObjects_[last++].swap(sharedPtr);
  }

  newGameObjects_.clear();
}

void Scene::onEvents() noexcept {
  // Clear the Input's cache
  Input::clear();

  // Handle all events
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      stop();
      return;
    }

    Input::update(event);
  }
}

void Scene::onUpdate() noexcept {
  size_t i = 0;
  while (i < gameObjects_.size()) {
    auto& entry = gameObjects_[i];
    entry->onUpdate();
    if (entry->destroy()) {
      gameObjects_.erase(gameObjects_.begin() + i);
      entry->onDestroy();
      entry.reset();
    } else {
      ++i;
    }
  }
}

void Scene::onLateUpdate() noexcept {
  world().Step(static_cast<float>(Time::delta()), 8, 3);

  for (auto& entry : gameObjects_) entry->onLateUpdate();
}

void Scene::onRender() noexcept {
  // Clear the screen
  SDL_RenderClear(Game::renderer());

  for (auto& entry : gameObjects_) entry->onRender();

  // Render the new frame
  SDL_RenderPresent(Game::renderer());
}

void Scene::onEnd() noexcept {
  for (auto& entry : newGameObjects_) entry.reset();
  newGameObjects_.clear();

  for (auto& entry : gameObjects_) entry.reset();
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

    auto gameObject = std::make_shared<GameObject>(shared_from_this());
    gameObject->load(object);
    gameObjects_.emplace_back(std::move(gameObject));
  }

  debug_print("Successfully loaded Scene '%s' with %zi GameObject(s).\n",
              name_.c_str(), gameObjects_.size());

  onStart();
}

void Scene::run() noexcept {
  stop_ = false;

  const constexpr static int32_t gameFrameRate = 60;
  const constexpr static double frameTime =
      1000.0 / static_cast<double>(gameFrameRate);

  world().SetContactListener(&contactListener_);

  size_t frame{0};
  while (!stop_) {
    const auto& start = Time::now();
    onCreate();
    onEvents();
    onUpdate();
    onLateUpdate();
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

  onEnd();
}

void Scene::end() noexcept { stop(); }

void Scene::stop() noexcept { stop_ = true; }

void Scene::addGameObject(GameObject* gameObject) noexcept {
  gameObjects_.emplace_back(gameObject);
}

void Scene::removeGameObject(GameObject* gameObject) noexcept {
  gameObject->destroy() = true;
}

std::weak_ptr<GameObject> Scene::getGameObjectByName(
    const std::string& name) const noexcept {
  for (const auto& object : gameObjects()) {
    if (object->name() == name) return object;
  }

  return std::weak_ptr<GameObject>();
}
