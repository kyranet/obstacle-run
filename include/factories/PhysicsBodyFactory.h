// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/PhysicsBody.h"
#include "interfaces/ComponentFactory.h"

class PhysicsBodyFactory final : public ComponentFactory<PhysicsBody> {
  enum class PhysicsBodyMask {
    Boundary = 1 << 0,
    Player = 1 << 1,
    Enemy = 1 << 2,
    Collectible = 1 << 3,
    Bullet = 1 << 4,
    Goal = 1 << 5
  };

  [[nodiscard]] static PhysicsBodyMask getBodyMaskFromName(
      const std::string& value) noexcept;
  [[nodiscard]] static uint16_t getMaskFromJson(
      const Json::Value& json) noexcept;
  [[nodiscard]] static Json::Value getJsonFromMask(uint16_t bits) noexcept;
  [[nodiscard]] static std::string getNameFromBodyType(
      b2BodyType value) noexcept;
  [[nodiscard]] static b2BodyType getBodyTypeFromName(
      const std::string& value) noexcept;

 public:
  PhysicsBodyFactory() noexcept;
  ~PhysicsBodyFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<PhysicsBody> fromJson(
      const Json::Value& json, std::weak_ptr<GameObject> parent) override;
  [[nodiscard]] Json::Value toJson(
      std::shared_ptr<PhysicsBody> value) const override;
};
