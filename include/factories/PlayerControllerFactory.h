// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "components/PlayerController.h"
#include "interfaces/ComponentFactory.h"

class PlayerControllerFactory final
    : public ComponentFactory<PlayerController> {
 public:
  PlayerControllerFactory() noexcept;
  ~PlayerControllerFactory() noexcept override;
};
