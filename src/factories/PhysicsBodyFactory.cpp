// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/PhysicsBodyFactory.h"

PhysicsBodyFactory::PhysicsBodyFactory() noexcept
    : ComponentFactory("PhysicsBody") {}

PhysicsBodyFactory::~PhysicsBodyFactory() noexcept = default;

// {
//   "name": "PhysicsBody",
//   "type": "static",
//   "sensor": false,
//   "density": 18.5,
//   "restitution": 0.6,
//   "linear_damping": 0,
//   "data": [400, 400, 100, 100],
//   "category": ["player"],
//   "mask": ["boundary"]
// }

std::shared_ptr<PhysicsBody> PhysicsBodyFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) {
  return std::make_shared<PhysicsBody>(
      parent, getBodyTypeFromName(json["type"].asString()),
      json["sensor"].asBool(), json["density"].asFloat(),
      json["restitution"].asFloat(), json["linear_damping"].asFloat(),
      Vector4<int32_t>(json["data"]), getMaskFromJson(json["category"]),
      getMaskFromJson(json["mask"]));
}

Json::Value PhysicsBodyFactory::toJson(
    std::shared_ptr<PhysicsBody> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["type"] = getNameFromBodyType(value->type());
  json["sensor"] = value->sensor();
  json["density"] = value->density();
  json["restitution"] = value->restitution();
  json["linear_damping"] = value->linearDamping();
  json["data"] = value->data().toJson();
  json["category"] = getJsonFromMask(value->category());
  json["mask"] = getJsonFromMask(value->mask());
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

PhysicsBodyFactory::PhysicsBodyMask PhysicsBodyFactory::getBodyMaskFromName(
    const std::string& value) noexcept {
  if (value == "boundary") return PhysicsBodyMask::Boundary;
  if (value == "player") return PhysicsBodyMask::Player;
  if (value == "enemy") return PhysicsBodyMask::Enemy;
  if (value == "collectible") return PhysicsBodyMask::Collectible;
  if (value == "bullet") return PhysicsBodyMask::Bullet;

  assert(((void)"'value' must be one of 'boundary', 'player', 'enemy', "
                "'collectible', 'bullet', or 'goal'.",
          value == "goal"));
  return PhysicsBodyMask::Goal;
}

uint16_t PhysicsBodyFactory::getMaskFromJson(const Json::Value& json) noexcept {
  assert(((void)"'json' must be an array.", json.isArray()));

  uint16_t bits = 0;
  for (const auto& mask : json) {
    assert(((void)"'mask' must be a string.", mask.isString()));
    bits |= static_cast<uint16_t>(getBodyMaskFromName(mask.asString()));
  }

  return bits;
}

Json::Value PhysicsBodyFactory::getJsonFromMask(const uint16_t bits) noexcept {
  Json::Value json(Json::arrayValue);
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Boundary))
    json.append("boundary");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Player))
    json.append("player");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Enemy))
    json.append("enemy");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Collectible))
    json.append("collectible");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Bullet))
    json.append("bullet");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Goal)) json.append("goal");
  return json;
}
