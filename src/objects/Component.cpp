// Copyright (c) 2020 Antonio Román. All rights reserved.
#include <objects/Component.h>

#include "objects/GameObject.h"

Component::Component() noexcept = default;
Component::Component(GameObject* gameObject) noexcept
    : gameObject_(gameObject) {}
Component::~Component() noexcept = default;

template <class T>
T Component::getComponent() const noexcept {
  return gameObject()->getComponent<T>();
}

template <class T>
T Component::getComponentInChildren() const noexcept {
  return gameObject()->getComponentInChildren<T>();
}

template <class T>
T Component::getComponentInParent() const noexcept {
  return gameObject()->getComponentInParent<T>();
}

void Component::destroy() noexcept {}

void Component::onAwake() noexcept { enabled() = true; }
void Component::onUpdate() noexcept {}
