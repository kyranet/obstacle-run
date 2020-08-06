// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <json/json.h>

#include <vector>

#include "SDL.h"
#include "utils/Vector2D.h"

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

  Vector2D<int> position_{0, 0};
  Vector2D<int> size_{0, 0};

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

  [[nodiscard]] inline SDL_Rect rectangle() const noexcept {
    return {position_.x() - size_.x() / 2, position_.y() - size_.y() / 2,
            size_.x(), size_.y()};
  }

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
  [[nodiscard]] T getComponent() const noexcept;

  template <class T>
  [[nodiscard]] T getComponentInChildren() const noexcept;

  template <class T>
  [[nodiscard]] T getComponentInParent() const noexcept;

  void load(const Json::Value& value);
  void onAwake() noexcept;
  void onUpdate() const noexcept;
  void onRender() noexcept;
  void onDestroy() noexcept;
};
