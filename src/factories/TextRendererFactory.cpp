// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/TextRendererFactory.h"

TextRendererFactory::TextRendererFactory() noexcept
    : ComponentFactory("TextRenderer") {}
TextRendererFactory::~TextRendererFactory() noexcept = default;

// {
//   "name": "TextRenderer",
//   "font": "PaperWorks",
//   "size": 16,
//   "text": "Something nice!"
// }

TextRenderer* TextRendererFactory::fromJson(const Json::Value& json) {
  return new TextRenderer(json["font"].asString(), json["text"].asString(),
                          json["size"].asInt());
}

Json::Value TextRendererFactory::toJson(TextRenderer* value) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  json["font"] = value->font();
  json["size"] = value->size();
  json["text"] = value->text();
  return json;
}