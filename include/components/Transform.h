// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_rect.h>

#include <cstdint>

#include "interfaces/JsonConvertible.h"
#include "objects/Component.h"
#include "utils/Vector2.h"

struct transform_patch_t : component_patch_t {
  Vector2<float> position;
  Vector2<int32_t> scale;
};

class Transform final : public Component {
  Vector2<float> position_;
  Vector2<int32_t> scale_;

 public:
  explicit Transform(std::weak_ptr<GameObject> gameObject) noexcept;
  ~Transform() noexcept override;

  [[nodiscard]] inline const Vector2<float>& position() const noexcept {
    return position_;
  }
  inline Vector2<float>& position() noexcept { return position_; }

  [[nodiscard]] inline const Vector2<int32_t>& scale() const noexcept {
    return scale_;
  }
  inline Vector2<int32_t>& scale() noexcept { return scale_; }

  [[nodiscard]] inline SDL_Rect rectangle() const noexcept {
    return {static_cast<int32_t>(position().x()),
            static_cast<int32_t>(position().y()), scale().x(), scale().y()};
  }

  [[nodiscard]] Json::Value toJson() const noexcept override;
  void patch(const Json::Value& json) noexcept override;
  void patch(const transform_patch_t& json) noexcept;
};
