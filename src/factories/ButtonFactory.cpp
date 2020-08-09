// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/ButtonFactory.h"

ButtonFactory::ButtonFactory() noexcept : ComponentFactory("Button") {}
ButtonFactory::~ButtonFactory() noexcept = default;

std::shared_ptr<Button> ButtonFactory::fromJson(const Json::Value&) {
  return std::make_shared<Button>();
}

Json::Value ButtonFactory::toJson(std::shared_ptr<Button>) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  return json;
}
