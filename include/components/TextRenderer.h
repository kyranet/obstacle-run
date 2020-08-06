// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL.h>

#include "objects/Component.h"

class Font;
class Transform;

class TextRenderer final : public Component {
  std::string font_;
  std::string text_;
  int size_;
  const Font* ttfFont_ = nullptr;
  const Transform* transform_ = nullptr;
  SDL_Texture* texture_ = nullptr;

 public:
  TextRenderer(std::string font, std::string text, int size) noexcept;
  ~TextRenderer() noexcept;

  void onAwake() noexcept override;
  void onUpdate() noexcept override;

  [[nodiscard]] inline const std::string& font() const noexcept {
    return font_;
  }
  inline std::string& font() noexcept { return font_; }

  [[nodiscard]] inline const std::string& text() const noexcept {
    return text_;
  }
  inline std::string& text() noexcept { return text_; }

  [[nodiscard]] inline const int& size() const noexcept { return size_; }
  inline int& size() noexcept { return size_; }
};

// {
//   "name": "TextRenderer",
//   "font": "PaperWorks",
//   "size": 16,
//   "text": "Something nice!"
// }