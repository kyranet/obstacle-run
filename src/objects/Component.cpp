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

template <class T>
T Component::getComponent() const noexcept {
  return getGameObject()->getComponent<T>();
}

template <class T>
T Component::getComponentInChildren() const noexcept {
  return getGameObject()->getComponentInChildren<T>();
}

template <class T>
T Component::getComponentInParent() const noexcept {
  return getGameObject()->getComponentInParent<T>();
}

void Component::destroy() noexcept {}

void Component::onAwake() noexcept { setEnabled(true); }
void Component::onUpdate() noexcept {}
