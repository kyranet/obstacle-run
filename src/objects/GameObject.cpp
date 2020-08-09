// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "objects/GameObject.h"

#include "components/Transform.h"
#include "managers/ComponentManager.h"
#include "utils/DebugAssert.h"

GameObject::GameObject() noexcept : parent_(nullptr) {}
GameObject::~GameObject() noexcept {
  for (auto* child : children()) {
    delete child;
  }
}

GameObject* GameObject::clickScan(SDL_Point point) const noexcept {
  for (auto it = children().rbegin(); it != children().rend(); ++it) {
    const auto child = *it;
    if (child->clickScan(point)) return child;
  }

  const auto rect = rectangle();
  return !transparent() && SDL_PointInRect(&point, &rect)
             ? const_cast<GameObject*>(this)
             : nullptr;
}

SDL_Rect GameObject::rectangle() const noexcept {
  const auto& tf = transform();
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
    auto* gameObject = new GameObject();
    gameObject->parent(this);
    gameObject->scene(scene_);
    children_.emplace_back(gameObject);
    gameObject->load(child);
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

  for (const auto& child : children()) {
    child->onAwake();
  }

  for (const auto& component : components()) {
    component->onAwake();
  }

  transform_ = getComponent<Transform*>();
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
