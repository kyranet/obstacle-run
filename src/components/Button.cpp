// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/Button.h"

#include <utility>

#include "components/TextRenderer.h"
#include "managers/Input.h"
#include "objects/GameObject.h"

Button::Button(std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)) {}

void Button::onUpdate() noexcept {
  Component::onUpdate();

  if (!Input::mouseButtonDown(MouseKey::LEFT)) return;

  const auto& go = gameObject().lock();
  const auto& clicked = go->clickScan(Input::rawMousePosition());
  if (clicked != go) return;

  // TODO: Do action
  const auto& text = go->getComponent<TextRenderer>();
  if (text == nullptr) return;

  if (text->color().z() == 0) {
    text->color() = Vector4<uint8_t>{255, 255, 255, 255};
  } else {
    text->color() = Vector4<uint8_t>{255, 0, 0, 255};
  }

  text->refresh();
}

Json::Value Button::toJson() const noexcept {
  Json::Value json(Json::objectValue);
  json["name"] = "Button";
  return json;
}