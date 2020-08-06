// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/ImageRendererFactory.h"

ImageRendererFactory::ImageRendererFactory() noexcept
    : ComponentFactory("ImageRenderer") {}

ImageRendererFactory::~ImageRendererFactory() noexcept = default;

// {
//   "name": "ImageRenderer",
//   "image": "ForestBackground",
//   "fit": "contain"
// }

ImageRenderer* ImageRendererFactory::fromJson(const Json::Value& json) {
  return new ImageRenderer(json["image"].asString());
}

Json::Value ImageRendererFactory::toJson(ImageRenderer* value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["image"] = value->path();
  return json;
}