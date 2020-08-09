// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PlayerController.h"

#include "components/Transform.h"
#include "managers/Input.h"
#include "objects/GameObject.h"
#include "utils/Time.h"
PlayerController::PlayerController(uint8_t speed) noexcept : speed_(speed) {}
PlayerController::~PlayerController() noexcept = default;

void PlayerController::onAwake() noexcept { Component::onAwake(); }
void PlayerController::onUpdate() noexcept {
  Component::onUpdate();

  if (Input::keyDown(KeyboardKey::W) || Input::keyDown(KeyboardKey::UP)) {
    gameObject()->transform().lock()->position().y() -=
        static_cast<float>(speed() * Time::delta());
  } else if (Input::keyDown(KeyboardKey::S) ||
             Input::keyDown(KeyboardKey::DOWN)) {
    gameObject()->transform().lock()->position().y() +=
        static_cast<float>(speed() * Time::delta());
  }

  if (Input::keyDown(KeyboardKey::A) || Input::keyDown(KeyboardKey::LEFT)) {
    gameObject()->transform().lock()->position().x() -=
        static_cast<float>(speed() * Time::delta());
  } else if (Input::keyDown(KeyboardKey::D) ||
             Input::keyDown(KeyboardKey::RIGHT)) {
    gameObject()->transform().lock()->position().x() +=
        static_cast<float>(speed() * Time::delta());
  }
}
