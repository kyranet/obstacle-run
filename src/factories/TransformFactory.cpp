// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/TransformFactory.h"

TransformFactory::TransformFactory() noexcept : ComponentFactory("Transform") {}
TransformFactory::~TransformFactory() noexcept = default;

// {
//   "name": "Transform",
//   "position": [0, 0],
//   "scale": [1, 1]
// }

Transform* TransformFactory::fromJson(const Json::Value& json) {
  return new Transform(Vector2<float>(json["position"]),
                       Vector2<int32_t>(json["scale"]));
}

Json::Value TransformFactory::toJson(Transform* value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["position"] = value->position().toJson();
  json["scale"] = value->scale().toJson();
  return json;
}
