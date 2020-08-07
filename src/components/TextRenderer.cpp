// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/TextRenderer.h"

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "managers/FontManager.h"
#include "objects/Font.h"
#include "objects/GameObject.h"

TextRenderer::TextRenderer(std::string font, std::string text, int size,
                           SDL_Color color) noexcept
    : font_(std::move(font)),
      text_(std::move(text)),
      size_(size),
      color_(color) {}
TextRenderer::~TextRenderer() noexcept { SDL_DestroyTexture(texture_); }

void TextRenderer::onAwake() noexcept {
  Component::onAwake();
  ttfFont_ = FontManager::instance()->get(font(), size());
  transform_ = gameObject()->getComponent<Transform*>();

  auto surface = ttfFont_->render(text().c_str(), color_, 2000);
  transform_->scale() = {surface->w, surface->h};
  rectangle_ = {0, 0, surface->w, surface->h};
  texture_ = SDL_CreateTextureFromSurface(Game::renderer(), surface);
  SDL_FreeSurface(surface);
}

void TextRenderer::onRender() noexcept {
  Component::onRender();

  const auto& src = rectangle();
  const auto& rect = transform_->rectangle();
  const auto& renderer = Game::renderer();
  SDL_RenderCopy(renderer, texture_, &src, &rect);
}
