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
    const Json::Value& json, std::weak_ptr<GameObject> parent) const noexcept {
  return std::make_shared<TextRenderer>(
      parent, json["font"].asString(), json["text"].asString(),
      static_cast<uint16_t>(json["size"].asUInt()),
      Vector4<uint8_t>(json["color"]));
}
