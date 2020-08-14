// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/BulletBoxFactory.h"

BulletBoxFactory::BulletBoxFactory() noexcept : ComponentFactory("BulletBox") {}
BulletBoxFactory::~BulletBoxFactory() noexcept = default;

// {
//   "name": "BulletBox",
//   "data": [50, 50, 25, 25],
//   "velocity": [35, 35],
//   "remaining": 2.5
// }

std::shared_ptr<BulletBox> BulletBoxFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) const noexcept {
  return std::make_shared<BulletBox>(parent, Vector4<int32_t>(json["data"]),
                                     Vector2<double>(json["velocity"]),
                                     json["remaining"].asDouble());
}
