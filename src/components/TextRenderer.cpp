// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/TextRenderer.h"

#include <SDL.h>

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "managers/FontManager.h"
#include "objects/Font.h"
#include "objects/GameObject.h"

TextRenderer::TextRenderer(std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)) {}
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

void TextRenderer::patch(const Json::Value& json) noexcept {
  Component::patch(json);

  font_ = json["font"].asString();
  text_ = json["text"].asString();
  size_ = static_cast<uint16_t>(json["size"].asUInt());
  color_ = Vector4<uint8_t>(json["color"]);

  ttfFont_ = FontManager::get(font(), size());
}
