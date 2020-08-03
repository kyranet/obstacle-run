// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "SceneManager.h"

#if _DEBUG
#include "../exceptions/PointerException.h"
#endif
#include "../scenes/Scene.h"

std::vector<std::pair<std::string, Scene*>> SceneManager::scenes_ = {};
size_t SceneManager::invalidIndex_ = ~(size_t)0;
size_t SceneManager::index_ = invalidIndex_;

size_t SceneManager::getSceneCount() noexcept { return scenes_.size(); }

Scene* SceneManager::createScene(const std::string& name) noexcept {
  auto* scene = new Scene(name);
  scenes_.emplace_back(scene->getName(), scene);
  return scene;
}

Scene* SceneManager::getActiveScene() noexcept {
  return index_ == invalidIndex_ ? nullptr : scenes_[index_].second;
}

Scene* SceneManager::getSceneAt(size_t position) noexcept {
  return scenes_[position].second;
}

Scene* SceneManager::getSceneByName(const std::string& name) noexcept {
  for (const auto& pair : scenes_) {
    if (pair.first == name) return pair.second;
  }
  return nullptr;
}

void SceneManager::loadScene(Scene* scene) {
#if _DEBUG
  if (scene == nullptr)
    throw PointerException(
        "'scene' from SceneManager::loadScene(Scene*) must not be nullptr.");
#endif
  // TODO: Add JSON to load scenes
}

void SceneManager::loadScene(const std::string& name) {
  loadScene(getSceneByName(name));
}

void SceneManager::moveGameObjectToScene(GameObject* object, Scene* scene) {
#if _DEBUG
  if (object == nullptr)
    throw PointerException(
        "'object' from SceneManager::moveGameObjectToScene(GameObject*, "
        "Scene*) must not be nullptr.");
  if (scene == nullptr)
    throw PointerException(
        "'scene' from SceneManager::moveGameObjectToScene(GameObject*, Scene*) "
        "must not be nullptr.");
#endif
  scene->addGameObject(object);
}

void SceneManager::setActiveScene(Scene* scene) noexcept {
  size_t i = 0;
  for (const auto& pair : scenes_) {
    if (pair.second == scene) {
      index_ = i;
      break;
    }

    ++i;
  }
}
