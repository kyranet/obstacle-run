// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <string>
#include <vector>

#include "objects/GameObject.h"

class Scene final {
 private:
  std::string name_;
  std::vector<GameObject*> newGameObjects_{};
  std::vector<GameObject*> gameObjects_{};
  bool stop_ = false;

  ~Scene() noexcept;
  void onStart() noexcept;
  void onCreate() noexcept;
  void onEvents() noexcept;
  void onUpdate() noexcept;
  void onRender() noexcept;
  void onEnd() noexcept;

 public:
  explicit Scene(const std::string& name) noexcept;

  void addGameObject(GameObject* gameObject) noexcept;
  void removeGameObject(GameObject* gameObject) noexcept;

  void load();
  void run() noexcept;
  void stop() noexcept;
  void end() noexcept;

  std::vector<GameObject*> getGameObjects() const noexcept;
  std::string getName() const noexcept;
};
