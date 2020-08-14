// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/TransformFactory.h"

TransformFactory::TransformFactory() noexcept : ComponentFactory("Transform") {}
TransformFactory::~TransformFactory() noexcept = default;

// {
//   "name": "Transform",
//   "position": [0, 0],
//   "scale": [1, 1]
// }

std::shared_ptr<Transform> TransformFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) const noexcept {
  return std::make_shared<Transform>(parent, Vector2<float>(json["position"]),
                                     Vector2<int32_t>(json["scale"]));
}
