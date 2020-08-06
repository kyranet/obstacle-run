// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <SDL.h>

#include "utils/Vector2D.h"

class Image {
  SDL_Texture* texture_ = nullptr;
  Vector2D<int> size_{};

 public:
  explicit Image(const char* path) noexcept;
  ~Image() noexcept;

  [[nodiscard]] inline SDL_Texture* render() noexcept { return texture_; }

  [[nodiscard]] inline const Vector2D<int>& size() const noexcept {
    return size_;
  }
};
