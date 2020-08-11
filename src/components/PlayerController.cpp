// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PlayerController.h"

#include <utility>

#include "components/PhysicsBody.h"
#include "components/Transform.h"
#include "managers/Input.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"
#include "utils/Time.h"

PlayerController::PlayerController(std::weak_ptr<GameObject> gameObject,
                                   uint8_t speed) noexcept
    : Component(std::move(gameObject)), speed_(speed) {}
PlayerController::~PlayerController() noexcept = default;

void PlayerController::onAwake() noexcept {
  Component::onAwake();
  physicsBody_ = gameObject().lock()->getComponent<PhysicsBody>();
}
void PlayerController::onUpdate() noexcept {
  Component::onUpdate();

  Vector2<float> translation;

  if (Input::keyDown(KeyboardKey::W) || Input::keyDown(KeyboardKey::UP)) {
    translation.y() -= 1.f;
  } else if (Input::keyDown(KeyboardKey::S) ||
             Input::keyDown(KeyboardKey::DOWN)) {
    translation.y() += 1.f;
  }

  if (Input::keyDown(KeyboardKey::A) || Input::keyDown(KeyboardKey::LEFT)) {
    translation.x() -= 1.f;
  } else if (Input::keyDown(KeyboardKey::D) ||
             Input::keyDown(KeyboardKey::RIGHT)) {
    translation.x() += 1.f;
  }

  if (translation.x() == 0.f && translation.y() == 0.f) {
    physicsBody_->body()->SetLinearVelocity({0.f, 0.f});
    return;
  }

  const auto s = speed();
  const auto m = translation.magnitude();
  translation = translation * s / m;
  physicsBody_->body()->SetLinearVelocity(translation.toVec());
}

void PlayerController::onLateUpdate() noexcept {
  Component::onLateUpdate();
  gameObject().lock()->transform().lock()->position() =
      physicsBody_->data().toVector2<float>();
}
