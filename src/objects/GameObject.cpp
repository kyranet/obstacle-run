// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "objects/GameObject.h"

#include <utility>

#include "components/Transform.h"
#include "managers/ComponentManager.h"
#include "utils/DebugAssert.h"

GameObject::GameObject(const Json::Value& json, std::weak_ptr<Scene> scene,
                       std::weak_ptr<GameObject> parent) noexcept
    : scene_(std::move(scene)), parent_(std::move(parent)) {
  load(json);
}

GameObject::~GameObject() noexcept {
  for (auto& child : children_) {
    child.reset();
  }
}

GameObject* GameObject::clickScan(SDL_Point point) const noexcept {
  for (auto it = children().rbegin(); it != children().rend(); ++it) {
    const auto& child = (*it).get();
    if (child->clickScan(point)) return child;
  }

  const auto rect = rectangle();
  return !transparent() && SDL_PointInRect(&point, &rect)
             ? const_cast<GameObject*>(this)
             : nullptr;
}

SDL_Rect GameObject::rectangle() const noexcept {
  const auto& tf = transform().lock();
  assert(((void)"'tf' must not be null!", tf));

  const auto& ps = tf->position();
  const auto& sz = tf->scale();
  return {static_cast<int>(ps.x()), static_cast<int>(ps.y()), sz.x(), sz.y()};
}

void GameObject::load(const Json::Value& value) {
  name() = value["name"].asString();

  const auto jsonChildren = value["children"];
  for (const auto& child : jsonChildren) {
    debug_print("Loading GameObject: '%s'.\n", child["name"].asCString());
    children_.emplace_back(
        std::make_unique<GameObject>(child, scene(), shared_from_this()));
  }

  const auto jsonComponents = value["components"];
  for (const auto& child : jsonComponents) {
    debug_print("Loading Component: '%s'.\n", child["name"].asCString());

    auto* factory = ComponentManager::get(child["name"].asString());
    assert(((void)"'factory' from GameObject::load(const Json::Value&) "
                  "must not be nullptr.",
            factory));
    auto* component = factory->fromJson(child);
    assert(((void)"'component' from GameObject::load(const Json::Value&) "
                  "must not be nullptr.",
            component));
    component->gameObject() = this;
    components_.emplace_back(component);
  }

  debug_print(
      "Successfully loaded GameObject '%s' with %zi Child(ren) and %zi "
      "Component(s).\n",
      name().c_str(), children().size(), components().size());
}

void GameObject::onAwake() noexcept {
  active() = true;
  transform_ = getComponent<Transform>();

  for (const auto& child : children()) {
    child->onAwake();
  }

  for (const auto& component : components()) {
    component->onAwake();
  }
}

void GameObject::onUpdate() const noexcept {
  for (const auto& child : children()) {
    if (child->active()) child->onUpdate();
  }

  for (const auto& component : components()) {
    if (component->enabled()) component->onUpdate();
  }
}

void GameObject::onRender() const noexcept {
  for (const auto& child : children()) {
    if (child->active()) child->onRender();
  }

  for (const auto& component : components()) {
    if (component->enabled()) component->onRender();
  }
}

void GameObject::onDestroy() noexcept { active() = false; }
