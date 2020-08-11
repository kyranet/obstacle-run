// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "components/BulletBox.h"
#include "interfaces/ComponentFactory.h"

class BulletBoxFactory final : public ComponentFactory<BulletBox> {
 public:
  BulletBoxFactory() noexcept;
  ~BulletBoxFactory() noexcept override;
  [[nodiscard]] std::shared_ptr<BulletBox> fromJson(
      const Json::Value& json, std::weak_ptr<GameObject> parent) override;
  [[nodiscard]] Json::Value toJson(
      std::shared_ptr<BulletBox> value) const override;
};
