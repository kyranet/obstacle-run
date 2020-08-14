// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/SolidRenderer.h"

#include <SDL.h>

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "objects/GameObject.h"

SolidRenderer::SolidRenderer(std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)) {}

SolidRenderer::~SolidRenderer() noexcept = default;

void SolidRenderer::onAwake() noexcept { Component::onAwake(); }

void SolidRenderer::onRender() noexcept {
  Component::onRender();
  gameObject().lock()->transform().lock()->scale() =
      Vector2{rectangle().a(), rectangle().z()};
  const auto destination = calculatedRectangle();
  SDL_SetRenderDrawColor(Game::renderer(), color().x(), color().y(),
                         color().z(), color().a());
  SDL_RenderFillRect(Game::renderer(), &destination);
}

SDL_Rect SolidRenderer::calculatedRectangle() const noexcept {
  return (rectangle() + gameObject().lock()->transform().lock()->position())
      .toRectangle();
}

Json::Value SolidRenderer::toJson() const noexcept {
  auto json = Component::toJson();
  json["name"] = "SolidRenderer";
  json["rectangle"] = rectangle().toJson();
  json["color"] = color().toJson();
  return json;
}

void SolidRenderer::patch(const Json::Value& json) noexcept {
  Component::patch(json);

  rectangle_ = Vector4<int32_t>(json["rectangle"]);
  color_ = Vector4<uint8_t>(json["color"]);
}
