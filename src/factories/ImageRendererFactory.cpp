// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/ImageRendererFactory.h"

#include <cassert>

ImageRendererFactory::ImageRendererFactory() noexcept
    : ComponentFactory("ImageRenderer") {}

ImageRendererFactory::~ImageRendererFactory() noexcept = default;

// {
//   "name": "ImageRenderer",
//   "image": "ForestBackground",
//   "fit": "contain"
// }

std::shared_ptr<ImageRenderer> ImageRendererFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) const noexcept {
  return std::make_shared<ImageRenderer>(
      parent, json["image"].asString(),
      getImageFitFromName(json["fit"].asString()));
}

ImageFit ImageRendererFactory::getImageFitFromName(
    const std::string& value) noexcept {
  if (value == "none") return ImageFit::None;
  if (value == "fill") return ImageFit::Fill;
  if (value == "contain") return ImageFit::Contain;

  assert(((void)"'value' must be one of 'none', 'fill', 'contain', or 'cover'.",
          value == "cover"));
  return ImageFit::Cover;
}
