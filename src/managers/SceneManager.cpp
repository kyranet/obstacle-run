// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "managers/SceneManager.h"

#include <json/json.h>

#include <cassert>

#include "scenes/Scene.h"
#include "utils/DebugAssert.h"

std::vector<std::pair<std::string, std::shared_ptr<Scene>>>
    SceneManager::scenes_ = {};
size_t SceneManager::invalidIndex_ = ~(size_t)0;
size_t SceneManager::index_ = invalidIndex_;

size_t SceneManager::getSceneCount() noexcept { return scenes_.size(); }

std::weak_ptr<Scene> SceneManager::createScene(
    const std::string& name) noexcept {
  auto scene = std::make_shared<Scene>(name);
  scenes_.emplace_back(std::make_pair(scene->name(), scene));
  return scene;
}

std::weak_ptr<Scene> SceneManager::getActiveScene() noexcept {
  return index_ == invalidIndex_ ? nullptr : scenes_[index_].second;
}

std::weak_ptr<Scene> SceneManager::getSceneAt(size_t position) noexcept {
  return scenes_[position].second;
}

std::weak_ptr<Scene> SceneManager::getSceneByName(
    const std::string& name) noexcept {
  for (const auto& pair : scenes_) {
    if (pair.first == name) return pair.second;
  }

  return std::weak_ptr<Scene>();
}

void SceneManager::loadScene(const std::weak_ptr<Scene>& scene) {
  assert((
      (void)"'scene' from SceneManager::loadScene(Scene*) must not be nullptr.",
      !scene.expired()));
  scene.lock()->load();
}

void SceneManager::loadScene(const std::string& name) {
  loadScene(getSceneByName(name));
}

void SceneManager::setActiveScene(const std::weak_ptr<Scene>& scene) noexcept {
  size_t i = 0;
  for (const auto& pair : scenes_) {
    if (scene.lock() == pair.second) {
      index_ = i;
      break;
    }

    ++i;
  }
}
