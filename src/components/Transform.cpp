// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/Transform.h"

#include <utility>

Transform::Transform(std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)) {}

Transform::~Transform() noexcept = default;

Json::Value Transform::toJson() const noexcept {
  auto json = Component::toJson();
  json["name"] = "Transform";
  json["position"] = position().toJson();
  json["scale"] = scale().toJson();
  return json;
}

void Transform::patch(const Json::Value& json) noexcept {
  patch({{json["id"].asUInt(), json["enabled"].asBool()},
         Vector2<float>(json["position"]),
         Vector2<int32_t>(json["scale"])});
}

void Transform::patch(const transform_patch_t& json) noexcept {
  Component::patch(json);

  position_ = json.position;
  scale_ = json.scale;
}
