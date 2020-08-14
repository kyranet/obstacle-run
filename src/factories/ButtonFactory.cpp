// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/ButtonFactory.h"

ButtonFactory::ButtonFactory() noexcept : ComponentFactory("Button") {}
ButtonFactory::~ButtonFactory() noexcept = default;

std::shared_ptr<Button> ButtonFactory::fromJson(
    const Json::Value&, std::weak_ptr<GameObject> parent) const noexcept {
  return std::make_shared<Button>(parent);
}
