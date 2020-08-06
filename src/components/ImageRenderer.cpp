// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/ImageRenderer.h"

#include <SDL_image.h>

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "managers/ImageManager.h"
#include "objects/GameObject.h"
#include "objects/Image.h"

ImageRenderer::ImageRenderer(std::string path) noexcept
    : path_(std::move(path)) {}
ImageRenderer::~ImageRenderer() noexcept = default;

void ImageRenderer::onAwake() noexcept {
  Component::onAwake();
  transform_ = gameObject()->getComponent<Transform*>();

  auto* image = ImageManager::instance()->get(path());
  texture_ = image->render();
  transform_->scale() = image->size();
}

void ImageRenderer::onUpdate() noexcept {
  Component::onUpdate();

  const auto& pt = transform_->position();
  const auto& sz = transform_->scale();

  const SDL_Rect src{0, 0, sz.x(), sz.y()};
  const SDL_Rect rect{pt.x(), pt.y(), sz.x(), sz.y()};
  const auto& renderer = Game::renderer();
  SDL_RenderCopy(renderer, texture_, &src, &rect);
}
