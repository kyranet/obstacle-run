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

  [[nodiscard]] inline const Vector2D<int>& position() const noexcept {
    return position_;
  }
  inline Vector2D<int>& position() noexcept { return position_; }

  [[nodiscard]] inline const Vector2D<int>& scale() const noexcept {
    return scale_;
  }
  inline Vector2D<int>& scale() noexcept { return scale_; }
};
