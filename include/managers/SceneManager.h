// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <string>
#include <vector>

class Scene;
class GameObject;

class SceneManager final {
  static std::vector<std::pair<std::string, Scene*>> scenes_;
  static size_t index_;
  static size_t invalidIndex_;

 public:
  SceneManager() = delete;
  ~SceneManager() = delete;

  [[nodiscard]] static size_t getSceneCount() noexcept;
  [[nodiscard]] static Scene* createScene(const std::string& name) noexcept;
  [[nodiscard]] static Scene* getActiveScene() noexcept;
  [[nodiscard]] static Scene* getSceneAt(size_t position) noexcept;
  [[nodiscard]] static Scene* getSceneByName(const std::string& name) noexcept;
  static void loadScene(Scene* scene);
  static void loadScene(const std::string& name);
  static void moveGameObjectToScene(GameObject* object, Scene* scene);
  static void setActiveScene(Scene* scene) noexcept;
};
