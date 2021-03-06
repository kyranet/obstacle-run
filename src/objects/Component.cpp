// Copyright (c) 2020 Antonio Román. All rights reserved.
#include <objects/Component.h>

#include <utility>

#include "objects/GameObject.h"

Component::Component(std::weak_ptr<GameObject> gameObject) noexcept
    : gameObject_(std::move(gameObject)) {}

Component::~Component() noexcept = default;

void Component::destroy() noexcept {
  enabled() = false;
  destroyed() = true;
}

void Component::onAwake() noexcept {}
void Component::onUpdate() noexcept {}
void Component::onLateUpdate() noexcept {}
void Component::onRender() noexcept {}

const std::weak_ptr<Scene>& Component::scene() const noexcept {
  return gameObject().lock()->scene();
}

Json::Value Component::toJson() const noexcept {
  Json::Value json(Json::objectValue);
  json["id"] = id();
  json["enabled"] = enabled();
  return json;
}
