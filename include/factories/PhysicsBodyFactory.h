// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/PhysicsBody.h"
#include "interfaces/ComponentFactory.h"

class PhysicsBodyFactory final : public ComponentFactory<PhysicsBody> {
  enum class PhysicsBodyMask {
    Boundary = 0x0001,
    Player = 0x0002,
    Enemy = 0x0004,
    Collectible = 0x0008,
    Goal = 0x0010
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
