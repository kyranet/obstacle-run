// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/PhysicsBody.h"
#include "interfaces/ComponentFactory.h"

class PhysicsBodyFactory final : public ComponentFactory<PhysicsBody> {
  static std::string getNameFromBodyType(b2BodyType value) noexcept;
  static b2BodyType getBodyTypeFromName(const std::string& value) noexcept;

 public:
  PhysicsBodyFactory() noexcept;
  ~PhysicsBodyFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<PhysicsBody> fromJson(
      const Json::Value& json, std::weak_ptr<GameObject> parent) override;
  [[nodiscard]] Json::Value toJson(
      std::shared_ptr<PhysicsBody> value) const override;
};
