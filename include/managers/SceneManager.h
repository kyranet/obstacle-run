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

  static size_t getSceneCount() noexcept;
  static Scene* createScene(const std::string& name) noexcept;
  static Scene* getActiveScene() noexcept;
  static Scene* getSceneAt(size_t position) noexcept;
  static Scene* getSceneByName(const std::string& name) noexcept;
  static void loadScene(Scene* scene);
  static void loadScene(const std::string& name);
  static void moveGameObjectToScene(GameObject* object, Scene* scene);
  static void setActiveScene(Scene* scene) noexcept;
};
