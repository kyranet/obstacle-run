// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "GameObject.h"

#include <utility>

#include "../managers/ComponentManager.h"
#include "../utils/DebugAssert.h"
#include "Component.h"

GameObject::GameObject() noexcept {}
GameObject::~GameObject() noexcept {
  for (auto* child : getChildren()) {
    delete child;
  }
}

std::string GameObject::getName() const noexcept { return name_; }
void GameObject::setName(std::string name) noexcept { name_ = std::move(name); }

bool GameObject::getActive() const noexcept { return active_; }
void GameObject::setActive(bool active) noexcept { active_ = active; }

bool GameObject::getTransparent() const noexcept { return transparent_; }
void GameObject::setTransparent(bool transparent) noexcept {
  transparent_ = transparent;
}

bool GameObject::getDestroy() const noexcept { return destroyed_; }
void GameObject::setDestroy(bool destroy) noexcept { destroyed_ = destroy; }

SDL_Rect GameObject::getRectangle() const noexcept {
  return {position_.getX() - size_.getX() / 2,
          position_.getY() - size_.getY() / 2, size_.getX(), size_.getY()};
}

GameObject* GameObject::clickScan(SDL_Point point) const noexcept {
  const auto children = getChildren();
  for (auto it = children.rbegin(); it != children.rend(); ++it) {
    const auto child = *it;
    if (child->clickScan(point)) return child;
  }

  const auto rect = getRectangle();
  return !getTransparent() && SDL_PointInRect(&point, &rect)
             ? const_cast<GameObject*>(this)
             : nullptr;
}

std::vector<GameObject*> GameObject::getChildren() const noexcept {
  return children_;
}

std::vector<Component*> GameObject::getComponents() const noexcept {
  return components_;
}

void GameObject::load(const Json::Value& value) {
  setName(value["name"].asString());

  const auto children = value["children"];
  for (const auto& child : children) {
    debug_print("Loading GameObject: '%s'.\n", child["name"].asCString());
    auto* gameObject = new GameObject();
    children_.emplace_back(gameObject);
    gameObject->load(child);
  }

  const auto components = value["components"];
  for (const auto& child : components) {
    debug_print("Loading Component: '%s'.\n", child["name"].asCString());

    auto* factory = ComponentManager::get(child["name"].asString());
    assert_not_null(factory,
                    "'factory' from GameObject::load(const Json::Value&) "
                    "must not be nullptr.");
    components_.emplace_back(factory->fromJson(child));
  }

  debug_print(
      "Successfully loaded GameObject '%s' with %zi Children and %zi "
      "Components.\n",
      getName().c_str(), getChildren().size(), getComponents().size());
}

void GameObject::onAwake() noexcept { setActive(true); }

void GameObject::onUpdate() noexcept {}

void GameObject::onRender() noexcept {}

void GameObject::onDestroy() noexcept {}
