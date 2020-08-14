// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/ImageRenderer.h"

#include <SDL_image.h>

#include <utility>

#include "Game.h"
#include "components/Transform.h"
#include "managers/ImageManager.h"
#include "objects/GameObject.h"
#include "objects/Image.h"

ImageRenderer::ImageRenderer(std::weak_ptr<GameObject> gameObject,
                             std::string path, ImageFit fit) noexcept
    : Component(std::move(gameObject)), path_(std::move(path)), fit_(fit) {}
ImageRenderer::~ImageRenderer() noexcept = default;

void ImageRenderer::onAwake() noexcept {
  Component::onAwake();

  image_ = ImageManager::get(path());
  texture_ = image_->render();
  updateImageFit();
}

void ImageRenderer::onRender() noexcept {
  Component::onRender();

  const auto& src = image()->rectangle();
  const auto& rect = gameObject().lock()->rectangle();
  const auto& renderer = Game::renderer();
  SDL_RenderCopy(renderer, texture_, &src, &rect);
}

void ImageRenderer::updateImageFit() noexcept {
  // ImageFit::None: image must fill the window, does not keep ratio:
  if (fit() == ImageFit::None) {
    gameObject().lock()->transform().lock()->scale() = image()->size();
    return;
  }

  const auto imageWidth = image()->size().x();
  const auto imageHeight = image()->size().y();

  int windowWidth = 0;
  int windowHeight = 0;
  SDL_GetWindowSize(Game::window(), &windowWidth, &windowHeight);

  const auto imageRatio =
      static_cast<float>(imageWidth) / static_cast<float>(imageHeight);
  const auto windowRatio =
      static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

  // ImageFit::Fill: image must fill the window, does not keep ratio:
  if (fit() == ImageFit::Fill || imageRatio == windowRatio) {
    gameObject().lock()->transform().lock()->scale() =
        Vector2{windowWidth, windowHeight};
    return;
  }

  // ImageFit::Contain: image must fit the window, keeps ratio:
  if (fit() == ImageFit::Contain) {
    // Image is wider than window
    if (imageRatio > windowRatio) {
      // Width must be window's width, calculate height keeping proportions
      gameObject().lock()->transform().lock()->scale() = Vector2{
          windowWidth,
          static_cast<int>(imageHeight * static_cast<float>(windowWidth) /
                           static_cast<float>(imageWidth))};
      return;
    }

    // Image is taller than window
    gameObject().lock()->transform().lock()->scale() =
        Vector2{static_cast<int>(imageWidth * static_cast<float>(windowHeight) /
                                 static_cast<float>(imageHeight)),
                windowHeight};
    return;
  }

  // ImageFit::Cover: image must fill the window, keeps ratio:
  // Image is wider than window, rescale height to fill:
  if (imageRatio > windowRatio) {
    // Height must be window's height, calculate width keeping proportions
    gameObject().lock()->transform().lock()->scale() =
        Vector2{static_cast<int>(imageWidth * static_cast<float>(windowHeight) /
                                 static_cast<float>(imageHeight)),
                windowHeight};
    return;
  }

  // Width must be window's width, calculate height keeping proportions
  gameObject().lock()->transform().lock()->scale() =
      Vector2{windowWidth,
              static_cast<int>(imageHeight * static_cast<float>(windowWidth) /
                               static_cast<float>(imageWidth))};
}

Json::Value ImageRenderer::toJson() const noexcept {
  Json::Value json(Json::objectValue);
  json["name"] = "ImageRenderer";
  json["image"] = path();
  json["fit"] = getNameFromImageFit(fit());
  return json;
}

std::string ImageRenderer::getNameFromImageFit(ImageFit value) noexcept {
  switch (value) {
    case ImageFit::None:
      return "none";
    case ImageFit::Fill:
      return "fill";
    case ImageFit::Contain:
      return "contain";
    default:
      return "cover";
  }
}
