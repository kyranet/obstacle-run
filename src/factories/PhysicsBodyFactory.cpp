// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/PhysicsBodyFactory.h"

PhysicsBodyFactory::PhysicsBodyFactory() noexcept
    : ComponentFactory("PhysicsBody") {}

PhysicsBodyFactory::~PhysicsBodyFactory() noexcept = default;

// {
//   "name": "PhysicsBody",
//   "type": "static",
//   "data": [400, 400, 100, 100]
// }

std::shared_ptr<PhysicsBody> PhysicsBodyFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) {
  return std::make_shared<PhysicsBody>(
      parent, getBodyTypeFromName(json["type"].asString()),
      Vector4<int32_t>(json["data"]));
}

Json::Value PhysicsBodyFactory::toJson(
    std::shared_ptr<PhysicsBody> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["type"] = getNameFromBodyType(value->type());
  json["data"] = value->data().toJson();
  return json;
}

std::string PhysicsBodyFactory::getNameFromBodyType(b2BodyType value) noexcept {
  switch (value) {
    case b2BodyType::b2_staticBody:
      return "static";
    case b2BodyType::b2_kinematicBody:
      return "kinematic";
    default:
      return "dynamic";
  }
}

b2BodyType PhysicsBodyFactory::getBodyTypeFromName(
    const std::string& value) noexcept {
  if (value == "static") return b2BodyType::b2_staticBody;
  if (value == "kinematic") return b2BodyType::b2_kinematicBody;

  assert(((void)"'value' must be one of 'static', 'kinematic', or 'dynamic'.",
          value == "dynamic"));
  return b2BodyType::b2_dynamicBody;
}