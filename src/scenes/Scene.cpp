#include "Scene.h"

#include <algorithm>

#include "../Game.h"
#include "../managers/Input.h"
#include "SDL.h"

Scene::Scene() noexcept = default;
Scene::~Scene() noexcept { end(); }

void Scene::onStart() {}

void Scene::onCreate() {
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
    for (auto gameObject : gameObjects_)
      if (gameObject->getActive()) gameObject->onEvent(event);
  }
}

void Scene::onUpdate() {
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

void Scene::onRender() {
  for (auto* entry : gameObjects_) entry->onRender();
}

void Scene::onEnd() {
  for (auto* entry : newGameObjects_) delete entry;
  newGameObjects_.clear();

  for (auto* entry : gameObjects_) delete entry;
  gameObjects_.clear();
}

void Scene::end() noexcept {
  stop();
  Game::getInstance()->end();
}

void Scene::stop() noexcept {
  stop_ = true;
  onEnd();
}

std::vector<GameObject*> Scene::getGameObjects() const noexcept {
  return gameObjects_;
}
