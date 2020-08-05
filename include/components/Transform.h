// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "interfaces/JsonConvertible.h"
#include "objects/Component.h"
#include "utils/Vector2D.h"

class Transform : public Component {
  Vector2D<int> position_;
  Vector2D<int> scale_;

 public:
  Transform(Vector2D<int> position, Vector2D<int> scale) noexcept;
  ~Transform() noexcept;

  Vector2D<int> getPosition() const noexcept;
  void setPosition(int x, int y) noexcept;
  void setPosition(Vector2D<int> position) noexcept;

  Vector2D<int> getScale() const noexcept;
  void setScale(Vector2D<int> scale) noexcept;
};
