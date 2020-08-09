// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/TextRenderer.h"

#include <SDL.h>

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "managers/FontManager.h"
#include "objects/Font.h"
#include "objects/GameObject.h"

TextRenderer::TextRenderer(std::string font, std::string text, uint16_t size,
                           Vector4<uint8_t> color) noexcept
    : font_(std::move(font)),
      text_(std::move(text)),
      size_(size),
      color_(std::move(color)) {}
TextRenderer::~TextRenderer() noexcept { SDL_DestroyTexture(texture_); }

void TextRenderer::refresh() noexcept {
  auto surface = ttfFont_->render(text().c_str(), color().toColor(), 2000);
  gameObject()->transform().lock()->scale() = Vector2{surface->w, surface->h};
  rectangle_ = Vector4{0, 0, surface->w, surface->h};
  texture_ = SDL_CreateTextureFromSurface(Game::renderer(), surface);
  SDL_FreeSurface(surface);
}

void TextRenderer::onAwake() noexcept {
  Component::onAwake();
  ttfFont_ = FontManager::instance()->get(font(), size());
  refresh();
}

void TextRenderer::onRender() noexcept {
  Component::onRender();

  const auto& src = rectangle().toRectangle();
  const auto& rect = gameObject()->rectangle();
  const auto& renderer = Game::renderer();
  SDL_RenderCopy(renderer, texture_, &src, &rect);
}
