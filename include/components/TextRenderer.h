// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_pixels.h>
#include <SDL_rect.h>

#include <cstdint>

#include "objects/Component.h"

struct SDL_Texture;
class Font;
class Transform;

class TextRenderer final : public Component {
  std::string font_;
  std::string text_;
  uint16_t size_;
  SDL_Color color_;
  SDL_Rect rectangle_{0, 0, 0, 0};
  const Font* ttfFont_ = nullptr;
  SDL_Texture* texture_ = nullptr;

 public:
  TextRenderer(std::string font, std::string text, uint16_t size,
               SDL_Color color) noexcept;
  ~TextRenderer() noexcept;

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

  [[nodiscard]] inline const SDL_Color& color() const noexcept {
    return color_;
  }
  inline SDL_Color& color() noexcept { return color_; }

  [[nodiscard]] inline const SDL_Rect& rectangle() const noexcept {
    return rectangle_;
  }

  void refresh() noexcept;
};