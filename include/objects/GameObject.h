// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <json/json.h>

#include <cstdint>
#include <vector>

#include "utils/Vector2.h"

struct SDL_Rect;
struct SDL_Point;
class Scene;
class Component;
class Transform;
class PhysicsBody;

class GameObject final : public std::enable_shared_from_this<GameObject> {
  uint32_t id_{0};
  bool active_{false};
  bool destroyed_{false};
  bool transparent_{false};
  std::string name_{};
  std::weak_ptr<Scene> scene_{};
  std::weak_ptr<GameObject> parent_{};
  std::weak_ptr<Transform> transform_{};
  std::weak_ptr<PhysicsBody> physics_{};
  std::vector<std::shared_ptr<GameObject>> children_{};
  std::vector<std::shared_ptr<Component>> components_{};

 public:
  explicit GameObject(std::weak_ptr<Scene> scene,
                      std::weak_ptr<GameObject> parent = {}) noexcept;
  ~GameObject() noexcept;
  void load(const Json::Value& value);

  [[nodiscard]] inline const std::string& name() const noexcept {
    return name_;
  }
  inline std::string& name() noexcept { return name_; }

  [[nodiscard]] inline const uint32_t& id() const noexcept { return id_; }
  inline uint32_t& id() noexcept { return id_; }

  [[nodiscard]] inline const std::weak_ptr<GameObject>& parent()
      const noexcept {
    return parent_;
  }
  inline std::weak_ptr<GameObject>& parent() noexcept { return parent_; }

  [[nodiscard]] inline const std::weak_ptr<Scene>& scene() const noexcept {
    return scene_;
  }
  inline std::weak_ptr<Scene>& scene() noexcept { return scene_; }

  [[nodiscard]] inline const std::weak_ptr<Transform>& transform()
      const noexcept {
    return transform_;
  }

  [[nodiscard]] inline const std::weak_ptr<PhysicsBody>& physics()
      const noexcept {
    return physics_;
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

  [[nodiscard]] std::shared_ptr<const GameObject> clickScan(
      SDL_Point point) const noexcept;

  [[nodiscard]] inline const std::vector<std::shared_ptr<GameObject>>&
  children() const noexcept {
    return children_;
  }

  [[nodiscard]] inline const std::vector<std::shared_ptr<Component>>&
  components() const noexcept {
    return components_;
  }

  inline void addChild(const std::shared_ptr<GameObject>& gameObject) noexcept {
    children_.emplace_back(gameObject);
  }

  inline void addComponent(
      const std::shared_ptr<Component>& component) noexcept {
    components_.emplace_back(component);
  }

  template <typename T>
  [[nodiscard]] inline std::shared_ptr<T> getComponent() const noexcept {
    for (const auto& component : components()) {
      auto casted = std::dynamic_pointer_cast<T>(component);
      if (casted) return casted;
    }

    return nullptr;
  }

  template <typename T>
  [[nodiscard]] inline std::shared_ptr<T> getComponentInChildren()
      const noexcept {
    for (const auto& child : children()) {
      auto component = child->getComponent<T>();
      if (component) return component;
    }

    return nullptr;
  }

  template <typename T>
  [[nodiscard]] inline std::shared_ptr<T> getComponentInParent()
      const noexcept {
    if (parent().expired()) return nullptr;
    return parent().lock()->getComponent<T>();
  }

  void onAwake() noexcept;
  void onUpdate() const noexcept;
  void onLateUpdate() noexcept;
  void onRender() const noexcept;
  void onDestroy() noexcept;

  [[nodiscard]] Json::Value toJson() const noexcept;
};
