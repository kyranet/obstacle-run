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
//   "color": { "red": 255, "green": 255, "blue": 255, "alpha": 1 }
// }

TextRenderer* TextRendererFactory::fromJson(const Json::Value& json) {
  const auto& rawColor = json["color"];
  return new TextRenderer(
      json["font"].asString(), json["text"].asString(), json["size"].asInt(),
      {static_cast<unsigned char>(rawColor["red"].asUInt()),
       static_cast<unsigned char>(rawColor["green"].asUInt()),
       static_cast<unsigned char>(rawColor["blue"].asUInt()),
       static_cast<unsigned char>(rawColor["alpha"].asUInt())});
}

Json::Value TextRendererFactory::toJson(TextRenderer* value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["font"] = value->font();
  json["size"] = value->size();
  json["text"] = value->text();

  Json::Value color(Json::objectValue);
  color["red"] = value->color().r;
  color["green"] = value->color().g;
  color["blue"] = value->color().b;
  color["alpha"] = value->color().a;

  json["color"] = color;
  return json;
}