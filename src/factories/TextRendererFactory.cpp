// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/TextRendererFactory.h"

TextRendererFactory::TextRendererFactory() noexcept
    : ComponentFactory("TextRenderer") {}
TextRendererFactory::~TextRendererFactory() noexcept = default;

// {
//   "name": "TextRenderer",
//   "font": "PaperWorks",
//   "size": 16,
//   "text": "Something nice!",
//   "color": [255, 255, 255, 255]
// }

std::shared_ptr<TextRenderer> TextRendererFactory::fromJson(
    const Json::Value& json) {
  return std::make_shared<TextRenderer>(
      json["font"].asString(), json["text"].asString(),
      static_cast<uint16_t>(json["size"].asUInt()),
      Vector4<uint8_t>(json["color"]));
}

Json::Value TextRendererFactory::toJson(
    std::shared_ptr<TextRenderer> value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["font"] = value->font();
  json["text"] = value->text();
  json["size"] = value->size();
  json["color"] = value->color().toJson();
  return json;
}