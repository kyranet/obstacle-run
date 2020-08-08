// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <string>
#include <vector>

class GameObject;

class Scene final {
 private:
  std::string name_;
  std::vector<GameObject*> newGameObjects_{};
  std::vector<GameObject*> gameObjects_{};
  bool stop_ = false;

  ~Scene() noexcept;
  void onStart() const noexcept;
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

  [[nodiscard]] inline const std::vector<GameObject*>& gameObjects()
      const noexcept {
    return gameObjects_;
  }

  [[nodiscard]] inline const std::string& name() const noexcept {
    return name_;
  }
};
