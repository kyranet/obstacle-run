// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

class GameObject;
class Scene;

class Component {
 private:
  bool enabled_{false};
  bool destroyed_{false};
  uint32_t id_{0};
  std::weak_ptr<GameObject> gameObject_{};

 public:
  explicit Component(std::weak_ptr<GameObject> gameObject) noexcept;
  virtual ~Component() noexcept;

  [[nodiscard]] inline const bool& enabled() const noexcept { return enabled_; }
  inline bool& enabled() noexcept { return enabled_; }

  [[nodiscard]] inline const bool& destroyed() const noexcept {
    return destroyed_;
  }
  inline bool& destroyed() noexcept { return destroyed_; }

  [[nodiscard]] inline const uint32_t& id() const noexcept { return id_; }
  inline uint32_t& id() noexcept { return id_; }

  [[nodiscard]] inline const std::weak_ptr<GameObject>& gameObject()
      const noexcept {
    return gameObject_;
  }

  [[nodiscard]] const std::weak_ptr<Scene>& scene() const noexcept;

  void destroy() noexcept;

  virtual void onAwake() noexcept;
  virtual void onUpdate() noexcept;
  virtual void onLateUpdate() noexcept;
  virtual void onRender() noexcept;
};
