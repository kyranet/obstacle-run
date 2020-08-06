// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <SDL_ttf.h>

class Font {
  TTF_Font* ttfFont_;

 public:
  Font(const char* filepath, int size) noexcept;
  ~Font() noexcept;

  [[nodiscard]] SDL_Surface* render(const char* text, const SDL_Color& color,
                                    unsigned int wrapLength) const noexcept;
};