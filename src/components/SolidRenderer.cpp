// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/SolidRenderer.h"

#include <SDL.h>

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "objects/GameObject.h"

SolidRenderer::SolidRenderer(std::weak_ptr<GameObject> gameObject,
                             const Vector4<int32_t>& rectangle,
                             const Vector4<uint8_t>& color) noexcept
    : Component(std::move(gameObject)), rectangle_(rectangle), color_(color) {}

SolidRenderer::~SolidRenderer() noexcept = default;

void SolidRenderer::onAwake() noexcept { Component::onAwake(); }

void SolidRenderer::onRender() noexcept {
  Component::onRender();

  const auto destination = calculatedRectangle();
  SDL_SetRenderDrawColor(Game::renderer(), color().x(), color().y(),
                         color().z(), color().a());
  SDL_RenderFillRect(Game::renderer(), &destination);
}

SDL_Rect SolidRenderer::calculatedRectangle() const noexcept {
  return (rectangle() + gameObject().lock()->transform().lock()->position())
      .toRectangle();
}
