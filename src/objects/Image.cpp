// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "objects/Image.h"

#include <SDL_image.h>

#include <cassert>

#include "Game.h"

Image::Image(const char* path) noexcept {
  auto* surface = IMG_Load(path);
  assert(((void)"'surface' from Image::Image(const char*) must not be nullptr.",
          surface));

  size_ = Vector2{surface->w, surface->h};
  texture_ = SDL_CreateTextureFromSurface(Game::renderer(), surface);
  assert(((void)"'texture' from Image::Image(const char*) must not be nullptr.",
          texture_));

  SDL_FreeSurface(surface);
}

Image::~Image() noexcept { SDL_DestroyTexture(texture_); }
