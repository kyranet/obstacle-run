// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/PhysicsBody.h"
#include "interfaces/ComponentFactory.h"

class PhysicsBodyFactory final : public ComponentFactory<PhysicsBody> {
  [[nodiscard]] static PhysicsBodyMask getBodyMaskFromName(
      const std::string& value) noexcept;
  [[nodiscard]] static uint16_t getMaskFromJson(
      const Json::Value& json) noexcept;
  [[nodiscard]] static b2BodyType getBodyTypeFromName(
      const std::string& value) noexcept;

 public:
  PhysicsBodyFactory() noexcept;
  ~PhysicsBodyFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<PhysicsBody> fromJson(
      const Json::Value& json,
      std::weak_ptr<GameObject> parent) const noexcept override;
};
