// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/Transform.h"

#include <utility>

Transform::Transform(std::weak_ptr<GameObject> gameObject,
                     Vector2<float> position, Vector2<int32_t> scale) noexcept
    : Component(std::move(gameObject)),
      position_(std::move(position)),
      scale_(std::move(scale)) {}

Transform::~Transform() noexcept = default;

Json::Value Transform::toJson() const noexcept {
  Json::Value json(Json::objectValue);
  json["name"] = "Transform";
  json["position"] = position().toJson();
  json["scale"] = scale().toJson();
  return json;
}