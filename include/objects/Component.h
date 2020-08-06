// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

class GameObject;

class Component {
 private:
  bool enabled_ = false;
  GameObject* gameObject_ = nullptr;

 public:
  Component() noexcept;
  explicit Component(GameObject* gameObject) noexcept;
  ~Component() noexcept;

  [[nodiscard]] inline const bool& enabled() const noexcept { return enabled_; }
  inline bool& enabled() noexcept { return enabled_; }

  [[nodiscard]] inline const GameObject* gameObject() const noexcept {
    return gameObject_;
  }
  inline GameObject*& gameObject() noexcept { return gameObject_; }

  template <class T>
  [[nodiscard]] T getComponent() const noexcept;

  template <class T>
  [[nodiscard]] T getComponentInChildren() const noexcept;

  template <class T>
  [[nodiscard]] T getComponentInParent() const noexcept;

  void destroy() noexcept;

  virtual void onAwake() noexcept;
  virtual void onUpdate() noexcept;
};
