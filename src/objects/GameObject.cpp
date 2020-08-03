// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "GameObject.h"

GameObject::GameObject() noexcept {}
GameObject::~GameObject() noexcept {
  for (auto* child : getChildren()) {
    delete child;
  }
}

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

void GameObject::onAwake() noexcept { setActive(true); }

void GameObject::onUpdate() noexcept {}

void GameObject::onRender() noexcept {}

void GameObject::onDestroy() noexcept {}
