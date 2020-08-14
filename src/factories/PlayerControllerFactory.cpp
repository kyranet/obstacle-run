// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/PlayerControllerFactory.h"

PlayerControllerFactory::PlayerControllerFactory() noexcept
    : ComponentFactory("PlayerController") {}
PlayerControllerFactory::~PlayerControllerFactory() noexcept = default;

std::shared_ptr<PlayerController> PlayerControllerFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) const noexcept {
  return std::make_shared<PlayerController>(
      parent, json["bullet_next"].asDouble(),
      static_cast<uint8_t>(json["bullet_clip"].asUInt()),
      static_cast<uint8_t>(json["speed"].asUInt()));
}
