// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "components/PlayerController.h"
#include "interfaces/ComponentFactory.h"

class PlayerControllerFactory final
    : public ComponentFactory<PlayerController> {
 public:
  PlayerControllerFactory() noexcept;
  ~PlayerControllerFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<PlayerController> fromJson(
      const Json::Value& json) override;
  [[nodiscard]] Json::Value toJson(
      std::shared_ptr<PlayerController> value) const override;
};
