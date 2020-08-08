// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_ttf.h>

#include <cstdint>
class Font {
  TTF_Font* ttfFont_;

 public:
  Font(const char* filepath, uint16_t size) noexcept;
  ~Font() noexcept;

  [[nodiscard]] SDL_Surface* render(const char* text, const SDL_Color& color,
                                    uint32_t wrapLength) const noexcept;
};