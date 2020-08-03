#pragma once

#include <vector>
class Scene;

class SceneManager {
  static std::vector<Scene*> scenes_;
  static size_t index_;
  static size_t invalidIndex_;

 public:
  static size_t getSceneCount() noexcept;
  static Scene* getActiveScene() noexcept;
  static Scene* getSceneAt(size_t position) noexcept;
  static void loadScene(Scene* scene) noexcept;
  static void setActiveScene(Scene* scene) noexcept;
};
