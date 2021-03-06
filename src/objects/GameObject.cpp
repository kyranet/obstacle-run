// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "objects/GameObject.h"

#include <utility>

#include "components/PhysicsBody.h"
#include "components/Transform.h"
#include "managers/ComponentManager.h"
#include "utils/DebugAssert.h"

GameObject::GameObject(std::weak_ptr<Scene> scene,
                       std::weak_ptr<GameObject> parent) noexcept
    : scene_(std::move(scene)), parent_(std::move(parent)) {}

GameObject::~GameObject() noexcept = default;

std::shared_ptr<const GameObject> GameObject::clickScan(
    SDL_Point point) const noexcept {
  for (auto it = children().rbegin(); it != children().rend(); ++it) {
    const auto& child = (*it);
    if (child->clickScan(point)) return child;
  }

  const auto rect = rectangle();
  return !transparent() && SDL_PointInRect(&point, &rect) ? shared_from_this()
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
  id() = value["id"].asUInt();
  name() = value["name"].asString();
  active() = value["active"].asBool();

  const auto jsonChildren = value["children"];
  for (const auto& child : jsonChildren) {
    debug_print("Loading GameObject: '%s'.\n", child["name"].asCString());
    auto gameObject = std::make_shared<GameObject>(scene(), shared_from_this());
    gameObject->load(child);
    children_.emplace_back(std::move(gameObject));
  }

  const auto jsonComponents = value["components"];
  for (const auto& child : jsonComponents) {
    debug_print("Loading Component: '%s'.\n", child["name"].asCString());

    auto factory = ComponentManager::get(child["name"].asString());
    assert(((void)"'factory' from GameObject::load(const Json::Value&) "
                  "must not be nullptr.",
            factory));

    components_.emplace_back(factory->fromJson(child, shared_from_this()));
  }

  debug_print(
      "Successfully loaded GameObject '%s' with %zi Child(ren) and %zi "
      "Component(s).\n",
      name().c_str(), children().size(), components().size());
}

void GameObject::onAwake() noexcept {
  transform_ = getComponent<Transform>();
  physics_ = getComponent<PhysicsBody>();

  for (const auto& child : children()) {
    if (child->active()) child->onAwake();
  }

  for (const auto& component : components()) {
    if (component->enabled()) component->onAwake();
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

void GameObject::onLateUpdate() noexcept {
  for (const auto& child : children()) {
    if (child->active()) child->onLateUpdate();
  }

  size_t childIndex = children().size();
  while (childIndex != 0) {
    const auto& child = children().at(--childIndex);
    if (child->destroyed()) {
      children_.erase(children().begin() + childIndex);
    }
  }

  for (const auto& component : components()) {
    if (component->enabled()) component->onLateUpdate();
  }

  size_t componentIndex = components().size();
  while (componentIndex != 0) {
    const auto& component = components().at(--componentIndex);
    if (component->destroyed()) {
      components_.erase(components().begin() + componentIndex);
    }
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

Json::Value GameObject::toJson() const noexcept {
  Json::Value json(Json::objectValue);
  json["id"] = id();
  json["active"] = active();

  Json::Value children(Json::arrayValue);
  for (const auto& child : children_) children.append(child->toJson());
  json["children"] = children;

  Json::Value components(Json::arrayValue);
  for (const auto& child : components_) components.append(child->toJson());
  json["components"] = components;

  return json;
}

void GameObject::destroy() noexcept {
  children_.clear();
  components_.clear();
  destroyed_ = true;
}
