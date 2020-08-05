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

  [[nodiscard]] bool getEnabled() const noexcept;
  void setEnabled(bool enabled) noexcept;

  [[nodiscard]] GameObject* getGameObject() const noexcept;
  void setGameObject() noexcept;

  template <typename T, typename = typename std::enable_if<
                            std::is_convertible<T, Component*>::value, T>::type>
  [[nodiscard]] T* getComponent() const noexcept;

  template <typename T, typename = typename std::enable_if<
                            std::is_convertible<T, Component*>::value, T>::type>
  [[nodiscard]] T* getComponentInChildren() const noexcept;

  template <typename T, typename = typename std::enable_if<
                            std::is_convertible<T, Component*>::value, T>::type>
  [[nodiscard]] T* getComponentInParent() const noexcept;

  void destroy() noexcept;

  void onUpdate() noexcept;
};
