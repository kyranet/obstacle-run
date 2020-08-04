// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "TransformFactory.h"

TransformFactory::TransformFactory() noexcept : ComponentFactory("Transform") {}
TransformFactory::~TransformFactory() noexcept = default;

// {
//   "name": "Transform",
//   "position": [0, 0],
//   "scale": [1, 1]
// }

Transform* TransformFactory::fromJson(const Json::Value& json) {
  return new Transform(Vector2D<int>::fromJson(json["position"]),
                       Vector2D<int>::fromJson(json["scale"]));
}

Json::Value TransformFactory::toJson(Transform* value) {
  Json::Value json(Json::objectValue);
  json["name"] = getName();
  json["position"] = value->getPosition().toJson();
  json["scale"] = value->getScale().toJson();
  return json;
}
