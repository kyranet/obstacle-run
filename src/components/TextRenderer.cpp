// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/TextRenderer.h"

#include <SDL.h>

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "managers/FontManager.h"
#include "objects/Font.h"
#include "objects/GameObject.h"

TextRenderer::TextRenderer(std::weak_ptr<GameObject> gameObject,
                           std::string font, std::string text, uint16_t size,
                           Vector4<uint8_t> color) noexcept
    : Component(std::move(gameObject)),
      font_(std::move(font)),
      text_(std::move(text)),
      size_(size),
      color_(std::move(color)) {}
TextRenderer::~TextRenderer() noexcept { SDL_DestroyTexture(texture_); }

void TextRenderer::refresh() noexcept {
  auto surface =
      ttfFont_.lock()->render(text().c_str(), color().toColor(), 2000);
  gameObject().lock()->transform().lock()->scale() =
      Vector2{surface->w, surface->h};
  rectangle_ = Vector4{0, 0, surface->w, surface->h};
  texture_ = SDL_CreateTextureFromSurface(Game::renderer(), surface);
  SDL_FreeSurface(surface);
}

void TextRenderer::onAwake() noexcept {
  Component::onAwake();
  ttfFont_ = FontManager::get(font(), size());
  refresh();
}

void TextRenderer::onRender() noexcept {
  Component::onRender();

  const auto& src = rectangle().toRectangle();
  const auto& rect = gameObject().lock()->rectangle();
  const auto& renderer = Game::renderer();
  SDL_RenderCopy(renderer, texture_, &src, &rect);
}

Json::Value TextRenderer::toJson() const noexcept {
  auto json = Component::toJson();
  json["name"] = "TextRenderer";
  json["font"] = font();
  json["text"] = text();
  json["size"] = size();
  json["color"] = color().toJson();
  return json;
}
