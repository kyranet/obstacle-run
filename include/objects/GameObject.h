// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <json/json.h>

#include <cstdint>
#include <vector>

#include "utils/Vector2D.h"

struct SDL_Rect;
struct SDL_Point;
class Component;
class Transform;

class GameObject final {
  bool active_ = false;
  bool destroyed_ = false;
  bool transparent_ = false;
  std::string name_{};
  GameObject* parent_ = nullptr;
  Transform* transform_ = nullptr;
  std::vector<GameObject*> children_{};
  std::vector<Component*> components_{};

  Vector2D<int32_t> position_{0, 0};
  Vector2D<int32_t> size_{0, 0};

 public:
  GameObject() noexcept;
  explicit GameObject(GameObject* parent) noexcept;
  ~GameObject() noexcept;

  [[nodiscard]] inline const std::string& name() const noexcept {
    return name_;
  }
  inline std::string& name() noexcept { return name_; }

  [[nodiscard]] inline const GameObject* parent() const noexcept {
    return parent_;
  }
  [[nodiscard]] inline const Transform* transform() const noexcept {
    return transform_;
  }

  [[nodiscard]] inline const bool& active() const noexcept { return active_; }
  inline bool& active() noexcept { return active_; }

  [[nodiscard]] inline const bool& transparent() const noexcept {
    return transparent_;
  }
  inline bool& transparent() noexcept { return transparent_; }

  [[nodiscard]] inline const bool& destroy() const noexcept {
    return destroyed_;
  }
  inline bool& destroy() noexcept { return destroyed_; }

  [[nodiscard]] SDL_Rect rectangle() const noexcept;

  [[nodiscard]] GameObject* clickScan(SDL_Point point) const noexcept;
  [[nodiscard]] inline const std::vector<GameObject*>& children()
      const noexcept {
    return children_;
  }
  [[nodiscard]] inline const std::vector<Component*>& components()
      const noexcept {
    return components_;
  }

  template <class T>
  [[nodiscard]] inline T getComponent() const noexcept {
    for (const auto& component : components()) {
      T casted = dynamic_cast<T>(component);
      if (casted) return casted;
    }

    return nullptr;
  }

  template <class T>
  [[nodiscard]] inline T getComponentInChildren() const noexcept {
    for (const auto& child : children()) {
      const auto* component = child->getComponent<T>();
      if (component) return component;
    }

    return nullptr;
  }

  template <class T>
  [[nodiscard]] inline T getComponentInParent() const noexcept {
    if (parent() == nullptr) return nullptr;

    return parent()->getComponent<T>();
  }

  void load(const Json::Value& value);
  void onAwake() noexcept;
  void onUpdate() const noexcept;
  void onRender() const noexcept;
  void onDestroy() noexcept;
};
