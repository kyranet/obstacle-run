// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/PlayerControllerFactory.h"

PlayerControllerFactory::PlayerControllerFactory() noexcept
    : ComponentFactory("PlayerController") {}
PlayerControllerFactory::~PlayerControllerFactory() noexcept = default;

std::shared_ptr<PlayerController> PlayerControllerFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) {
  return std::make_shared<PlayerController>(
      parent, static_cast<uint8_t>(json["speed"].asUInt()));
}

Json::Value PlayerControllerFactory::toJson(
    std::shared_ptr<PlayerController> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["speed"] = value->speed();
  return json;
}
