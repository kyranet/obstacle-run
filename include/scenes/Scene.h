// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <box2d/box2d.h>

#include <memory>
#include <string>
#include <vector>

#include "listeners/ContactListener.h"
#include "objects/GameObject.h"

class Scene final : public std::enable_shared_from_this<Scene> {
 private:
  std::string name_;
  std::vector<std::unique_ptr<GameObject>> newGameObjects_{};
  std::vector<std::shared_ptr<GameObject>> gameObjects_{};
  b2World world_;
  ContactListener contactListener_;
  bool stop_ = false;

  void onStart() const noexcept;
  void onCreate() noexcept;
  void onEvents() noexcept;
  void onUpdate() noexcept;
  void onLateUpdate() noexcept;
  void onRender() noexcept;
  void onEnd() noexcept;

 public:
  explicit Scene(std::string name) noexcept;
  ~Scene() noexcept;

  void addGameObject(GameObject* gameObject) noexcept;
  void removeGameObject(GameObject* gameObject) noexcept;
  [[nodiscard]] std::weak_ptr<GameObject> getGameObjectByName(
      const std::string& name) const noexcept;

  void load();
  void run() noexcept;
  void stop() noexcept;
  void end() noexcept;

  [[nodiscard]] inline const std::vector<std::shared_ptr<GameObject>>&
  gameObjects() const noexcept {
    return gameObjects_;
  }

  [[nodiscard]] inline b2World& world() noexcept { return world_; }

  [[nodiscard]] inline const std::string& name() const noexcept {
    return name_;
  }
};
