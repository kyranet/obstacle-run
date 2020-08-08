// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_rect.h>

#include <cstdint>

#include "interfaces/JsonConvertible.h"
#include "objects/Component.h"
#include "utils/Vector2.h"

class Transform final : public Component {
  Vector2<int32_t> position_;
  Vector2<int32_t> scale_;

 public:
  Transform(Vector2<int32_t> position, Vector2<int32_t> scale) noexcept;
  ~Transform() noexcept;

  [[nodiscard]] inline const Vector2<int32_t>& position() const noexcept {
    return position_;
  }
  inline Vector2<int32_t>& position() noexcept { return position_; }

  [[nodiscard]] inline const Vector2<int32_t>& scale() const noexcept {
    return scale_;
  }
  inline Vector2<int32_t>& scale() noexcept { return scale_; }

  [[nodiscard]] inline SDL_Rect rectangle() const noexcept {
    return {position().x(), position().y(), scale().x(), scale().y()};
  }
};
