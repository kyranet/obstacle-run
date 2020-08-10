// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/PhysicsBodyFactory.h"

PhysicsBodyFactory::PhysicsBodyFactory() noexcept
    : ComponentFactory("PhysicsBody") {}

PhysicsBodyFactory::~PhysicsBodyFactory() noexcept = default;

// {
//   "name": "PhysicsBody",
//   "data": [400, 400, 100, 100]
// }

std::shared_ptr<PhysicsBody> PhysicsBodyFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) {
  return std::make_shared<PhysicsBody>(parent, Vector4<int32_t>(json["data"]));
}

Json::Value PhysicsBodyFactory::toJson(
    std::shared_ptr<PhysicsBody> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["data"] = value->data().toJson();
  return json;
}