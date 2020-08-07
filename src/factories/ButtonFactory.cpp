// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/ButtonFactory.h"

ButtonFactory::ButtonFactory() noexcept : ComponentFactory("Button") {}
ButtonFactory::~ButtonFactory() noexcept = default;

Button* ButtonFactory::fromJson(const Json::Value&) { return new Button(); }

Json::Value ButtonFactory::toJson(Button*) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  return json;
}
