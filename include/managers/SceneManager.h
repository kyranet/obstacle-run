// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <memory>
#include <string>
#include <vector>

class Scene;
class GameObject;

class SceneManager final {
  static std::vector<std::pair<std::string, std::shared_ptr<Scene>>> scenes_;
  static size_t index_;
  static size_t invalidIndex_;

 public:
  SceneManager() = delete;
  ~SceneManager() = delete;

  [[nodiscard]] static size_t getSceneCount() noexcept;
  [[nodiscard]] static std::weak_ptr<Scene> createScene(
      const std::string& name) noexcept;
  [[nodiscard]] static std::weak_ptr<Scene> getActiveScene() noexcept;
  [[nodiscard]] static std::weak_ptr<Scene> getSceneAt(
      size_t position) noexcept;
  [[nodiscard]] static std::weak_ptr<Scene> getSceneByName(
      const std::string& name) noexcept;
  static void loadScene(const std::weak_ptr<Scene>& scene);
  static void loadScene(const std::string& name);
  static void setActiveScene(const std::weak_ptr<Scene>& scene) noexcept;
};
