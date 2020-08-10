// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "components/Transform.h"
#include "interfaces/ComponentFactory.h"

class TransformFactory final : public ComponentFactory<Transform> {
 public:
  TransformFactory() noexcept;
  ~TransformFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<Transform> fromJson(
      const Json::Value& json, std::weak_ptr<GameObject> parent) override;
  [[nodiscard]] Json::Value toJson(
      std::shared_ptr<Transform> value) const override;
};
