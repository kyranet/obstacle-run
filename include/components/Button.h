// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"

class Button : public Component {
 public:
  void onUpdate() noexcept override;
};
