// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

class GameObject;
class Scene;

class Component {
 private:
  bool enabled_ = false;
  std::weak_ptr<GameObject> gameObject_{};

 public:
  explicit Component(std::weak_ptr<GameObject> gameObject) noexcept;
  virtual ~Component() noexcept;

  [[nodiscard]] inline const bool& enabled() const noexcept { return enabled_; }
  inline bool& enabled() noexcept { return enabled_; }

  [[nodiscard]] inline const std::weak_ptr<GameObject>& gameObject()
      const noexcept {
    return gameObject_;
  }

  [[nodiscard]] const std::weak_ptr<Scene>& scene() const noexcept;

  void destroy() noexcept;

  virtual void onAwake() noexcept;
  virtual void onUpdate() noexcept;
  virtual void onRender() noexcept;
};
