// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "RendererFactory.h"

RendererFactory::RendererFactory() noexcept : ComponentFactory("Renderer") {}
RendererFactory::~RendererFactory() noexcept = default;

// {
//   "name": "Renderer"
// }

Renderer* RendererFactory::fromJson(const Json::Value& json) { return {}; }

Json::Value RendererFactory::toJson(Renderer* value) {
  Json::Value json(Json::objectValue);
  json["name"] = getName();
  return json;
}