// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "components/Button.h"
#include "interfaces/ComponentFactory.h"

class ButtonFactory final : public ComponentFactory<Button> {
 public:
  ButtonFactory() noexcept;
  ~ButtonFactory() noexcept override;
};
