// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/RendererFactory.h"

RendererFactory::RendererFactory() noexcept : ComponentFactory("Renderer") {}

RendererFactory::~RendererFactory() noexcept = default;

// {
//   "name": "Renderer"
// }

Renderer* RendererFactory::fromJson(const Json::Value&) { return {}; }

Json::Value RendererFactory::toJson(Renderer*) {
  Json::Value json(Json::objectValue);
  json["name"] = getName();
  return json;
}