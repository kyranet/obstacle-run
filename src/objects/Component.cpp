// Copyright (c) 2020 Antonio Román. All rights reserved.
#include <objects/Component.h>

#include "objects/GameObject.h"

Component::Component() noexcept = default;
Component::Component(GameObject* gameObject) noexcept
    : gameObject_(gameObject) {}
Component::~Component() noexcept = default;

bool Component::getEnabled() const noexcept { return enabled_; }
void Component::setEnabled(bool enabled) noexcept { enabled_ = enabled; }

GameObject* Component::getGameObject() const noexcept { return gameObject_; }
void Component::setGameObject() noexcept { gameObject_ = nullptr; }

template <typename T, typename>
T* Component::getComponent() const noexcept {
  for (const auto& component : getGameObject()->getComponents()) {
    if (dynamic_cast<T>(component)) return component;
  }

  return nullptr;
}

template <typename T, typename>
T* Component::getComponentInChildren() const noexcept {
  for (const auto& child : getGameObject()->getChildren()) {
    for (const auto& component : child->getComponents()) {
      if (dynamic_cast<T>(component)) return component;
    }
  }

  return nullptr;
}
template <typename T, typename>
T* Component::getComponentInParent() const noexcept {
  const auto* parent = getGameObject()->getParent();
  if (parent == nullptr) return nullptr;

  for (const auto& component : parent->getComponents()) {
    if (dynamic_cast<T>(component)) return component;
  }

  return nullptr;
}

void Component::destroy() noexcept {}

void Component::onAwake() noexcept { setEnabled(true); }
void Component::onUpdate() noexcept {}
