// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/Button.h"

#include "components/TextRenderer.h"
#include "managers/Input.h"
#include "objects/GameObject.h"

void Button::onUpdate() noexcept {
  Component::onUpdate();

  if (!Input::mouseButtonDown(MouseKey::LEFT)) return;

  const auto& clicked = gameObject()->clickScan(Input::rawMousePosition());
  if (clicked != gameObject()) return;

  // TODO: Do action
  const auto& text = gameObject()->getComponent<TextRenderer*>();
  if (text == nullptr) return;

  if (text->color().g == 0) {
    text->color() = {255, 255, 255, 255};
  } else {
    text->color() = {255, 0, 0, 255};
  }

  text->refresh();
}
