// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_rect.h>

#include "objects/Component.h"
#include "utils/Vector4.h"
class Transform;

struct solid_renderer_patch_t : component_patch_t {
  Vector4<int32_t> rectangle;
  Vector4<uint8_t> color;
};

class SolidRenderer final : public Component {
  Vector4<int32_t> rectangle_;
  Vector4<uint8_t> color_;

 public:
  explicit SolidRenderer(std::weak_ptr<GameObject> gameObject) noexcept;
  ~SolidRenderer() noexcept override;

  void onAwake() noexcept override;
  void onRender() noexcept override;

  [[nodiscard]] inline const Vector4<int32_t>& rectangle() const noexcept {
    return rectangle_;
  }
  inline Vector4<int32_t>& rectangle() noexcept { return rectangle_; }

  [[nodiscard]] SDL_Rect calculatedRectangle() const noexcept;

  [[nodiscard]] inline const Vector4<uint8_t>& color() const noexcept {
    return color_;
  }
  inline Vector4<uint8_t>& color() noexcept { return color_; }

  [[nodiscard]] Json::Value toJson() const noexcept override;
  void patch(const Json::Value& json) noexcept override;
  void patch(const solid_renderer_patch_t& json) noexcept;
};
