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

std::shared_ptr<SolidRenderer> SolidRendererFactory::fromJson(
    const Json::Value& json, std::weak_ptr<GameObject> parent) {
  return std::make_shared<SolidRenderer>(parent,
                                         Vector4<int32_t>(json["rectangle"]),
                                         Vector4<uint8_t>(json["color"]));
}

Json::Value SolidRendererFactory::toJson(
    std::shared_ptr<SolidRenderer> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["rectangle"] = value->rectangle().toJson();
  json["color"] = value->color().toJson();
  return json;
}
