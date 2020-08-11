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

Json::Value BulletBoxFactory::toJson(std::shared_ptr<BulletBox> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["data"] = value->data().toJson();
  json["velocity"] = value->velocity().toJson();
  json["remaining"] = value->remaining();
  return json;
}

std::shared_ptr<BulletBox> BulletBoxFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) {
  return std::make_shared<BulletBox>(parent, Vector4<int32_t>(json["data"]),
                                     Vector2<double>(json["velocity"]),
                                     json["remaining"].asDouble());
}
