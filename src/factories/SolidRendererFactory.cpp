// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/SolidRendererFactory.h"

#include <cassert>

SolidRendererFactory::SolidRendererFactory() noexcept
    : ComponentFactory("SolidRenderer") {}

SolidRendererFactory::~SolidRendererFactory() noexcept = default;

// {
//   "name": "ImageRenderer",
//   "rectangle": [0, 0, 500, 500],
//   "color": [255, 255, 0, 255]
// }

SolidRenderer* SolidRendererFactory::fromJson(const Json::Value& json) {
  return new SolidRenderer(Vector4<int32_t>(json["rectangle"]),
                           Vector4<uint8_t>("color"));
}

Json::Value SolidRendererFactory::toJson(SolidRenderer* value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["rectangle"] = value->rectangle().toJson();
  json["color"] = value->color().toJson();
  return json;
}
