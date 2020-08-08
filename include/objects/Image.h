// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_rect.h>

#include <cstdint>

#include "utils/Vector2D.h"

struct SDL_Texture;

class Image {
  SDL_Texture* texture_ = nullptr;
  Vector2D<int32_t> size_{};

 public:
  explicit Image(const char* path) noexcept;
  ~Image() noexcept;

  [[nodiscard]] inline SDL_Texture* render() noexcept { return texture_; }

  [[nodiscard]] inline const Vector2D<int32_t>& size() const noexcept {
    return size_;
  }

  [[nodiscard]] inline SDL_Rect rectangle() const noexcept {
    return {0, 0, size().x(), size().y()};
  }
};
