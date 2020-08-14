// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/PhysicsBody.h"
#include "interfaces/ComponentFactory.h"

class PhysicsBodyFactory final : public ComponentFactory<PhysicsBody> {
 public:
  PhysicsBodyFactory() noexcept;
  ~PhysicsBodyFactory() noexcept override;
};
