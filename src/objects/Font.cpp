// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "objects/Font.h"

#include <cassert>
#include <cstdio>

Font::Font(const char* filepath, uint16_t size) noexcept {
  ttfFont_ = TTF_OpenFont(filepath, size);
  assert(((void)"'ttfFont_' from Font::Font(const char*, uint16_t) must not be "
                "nullptr.",
          ttfFont_));
}

Font::~Font() noexcept { TTF_CloseFont(ttfFont_); }

SDL_Surface* Font::render(const char* text, const SDL_Color& color,
                          uint32_t wrapLength) const noexcept {
  return TTF_RenderText_Blended_Wrapped(ttfFont_, text, color, wrapLength);
}
