// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/PlayerControllerFactory.h"

PlayerControllerFactory::PlayerControllerFactory() noexcept
    : ComponentFactory("PlayerController") {}
PlayerControllerFactory::~PlayerControllerFactory() noexcept = default;

PlayerController* PlayerControllerFactory::fromJson(const Json::Value& json) {
  return new PlayerController(static_cast<uint8_t>(json["speed"].asUInt()));
}

Json::Value PlayerControllerFactory::toJson(PlayerController* value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["speed"] = value->speed();
  return json;
}
