#include "SceneManager.h"
std::vector<Scene*> SceneManager::scenes_ = {};
size_t SceneManager::invalidIndex_ = ~(size_t)0;
size_t SceneManager::index_ = invalidIndex_;

size_t SceneManager::getSceneCount() noexcept { return scenes_.size(); }

Scene* SceneManager::getActiveScene() noexcept {
  return index_ == invalidIndex_ ? nullptr : scenes_[index_];
}

Scene* SceneManager::getSceneAt(size_t position) noexcept {
  return scenes_[position];
}

void SceneManager::loadScene(Scene* scene) noexcept {
  scenes_.push_back(scene);
}

void SceneManager::setActiveScene(Scene* scene) noexcept {
  size_t i = 0;
  for (const auto* entry : scenes_) {
    if (entry == scene) {
      index_ = i;
      break;
    }
  }
}
