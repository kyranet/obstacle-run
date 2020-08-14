// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_pixels.h>
#include <SDL_rect.h>

#include <cstdint>

#include "objects/Component.h"
#include "utils/Vector4.h"

struct SDL_Texture;
class Font;
class Transform;

class TextRenderer final : public Component {
  std::string font_{};
  std::string text_{};
  uint16_t size_{};
  Vector4<int32_t> rectangle_{};
  Vector4<uint8_t> color_{};
  std::weak_ptr<Font> ttfFont_{};
  SDL_Texture* texture_ = nullptr;

 public:
  explicit TextRenderer(std::weak_ptr<GameObject> gameObject) noexcept;
  ~TextRenderer() noexcept override;

  void onAwake() noexcept override;
  void onRender() noexcept override;

  [[nodiscard]] inline const std::string& font() const noexcept {
    return font_;
  }
  inline std::string& font() noexcept { return font_; }

  [[nodiscard]] inline const std::string& text() const noexcept {
    return text_;
  }
  inline std::string& text() noexcept { return text_; }

  [[nodiscard]] inline const uint16_t& size() const noexcept { return size_; }
  inline uint16_t& size() noexcept { return size_; }

  [[nodiscard]] inline const Vector4<uint8_t>& color() const noexcept {
    return color_;
  }
  inline Vector4<uint8_t>& color() noexcept { return color_; }

  [[nodiscard]] inline const Vector4<int32_t>& rectangle() const noexcept {
    return rectangle_;
  }

  void refresh() noexcept;

  [[nodiscard]] Json::Value toJson() const noexcept override;
  void patch(const Json::Value& json) noexcept override;
};