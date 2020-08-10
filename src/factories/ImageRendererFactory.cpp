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
    const Json::Value& json, std::weak_ptr<GameObject> parent) {
  return std::make_shared<ImageRenderer>(
      parent, json["image"].asString(),
      getImageFitFromName(json["fit"].asString()));
}

Json::Value ImageRendererFactory::toJson(
    std::shared_ptr<ImageRenderer> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["image"] = value->path();
  json["fit"] = getNameFromImageFit(value->fit());
  return json;
}

std::string ImageRendererFactory::getNameFromImageFit(ImageFit value) noexcept {
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

ImageFit ImageRendererFactory::getImageFitFromName(
    const std::string& value) noexcept {
  if (value == "none") return ImageFit::None;
  if (value == "fill") return ImageFit::Fill;
  if (value == "contain") return ImageFit::Contain;

  assert(((void)"'value' must be one of 'none', 'fill', 'contain', or 'cover'.",
          value == "cover"));
  return ImageFit::Cover;
}
