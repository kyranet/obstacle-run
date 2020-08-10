// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <memory>
#include <string>
#include <vector>

#include "objects/GameObject.h"

class Scene final : public std::enable_shared_from_this<Scene> {
 private:
  std::string name_;
  std::vector<std::unique_ptr<GameObject>> newGameObjects_{};
  std::vector<std::shared_ptr<GameObject>> gameObjects_{};
  bool stop_ = false;

  void onStart() const noexcept;
  void onCreate() noexcept;
  void onEvents() noexcept;
  void onUpdate() noexcept;
  void onRender() noexcept;
  void onEnd() noexcept;

 public:
  explicit Scene(const std::string& name) noexcept;
  ~Scene() noexcept;

  void addGameObject(GameObject* gameObject) noexcept;
  void removeGameObject(GameObject* gameObject) noexcept;

  void load();
  void run() noexcept;
  void stop() noexcept;
  void end() noexcept;

  [[nodiscard]] inline const std::vector<std::shared_ptr<GameObject>>&
  gameObjects() const noexcept {
    return gameObjects_;
  }

  [[nodiscard]] inline const std::string& name() const noexcept {
    return name_;
  }
};
