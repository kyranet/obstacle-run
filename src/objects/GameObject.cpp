// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "objects/GameObject.h"

#include "components/Transform.h"
#include "managers/ComponentManager.h"
#include "utils/DebugAssert.h"

GameObject::GameObject() noexcept : parent_(nullptr) {}
GameObject::GameObject(GameObject* parent) noexcept : parent_(parent) {}
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

template <class T>
T GameObject::getComponent() const noexcept {
  for (const auto& component : components()) {
    T casted = dynamic_cast<T>(component);
    if (casted) return casted;
  }

  return nullptr;
}

template <class T>
T GameObject::getComponentInChildren() const noexcept {
  for (const auto& child : children()) {
    const auto* component = child->getComponent<T>();
    if (component) return component;
  }

  return nullptr;
}
template <class T>
T GameObject::getComponentInParent() const noexcept {
  if (parent() == nullptr) return nullptr;

  return parent()->getComponent<T>();
}

void GameObject::load(const Json::Value& value) {
  name() = value["name"].asString();

  const auto jsonChildren = value["children"];
  for (const auto& child : jsonChildren) {
    debug_print("Loading GameObject: '%s'.\n", child["name"].asCString());
    auto* gameObject = new GameObject(this);
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

void GameObject::onRender() noexcept {}

void GameObject::onDestroy() noexcept { active() = false; }
