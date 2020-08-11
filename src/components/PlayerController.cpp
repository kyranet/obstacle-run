// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PlayerController.h"

#include <utility>

#if !NDEBUG
#include "Game.h"
#endif
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
  } else {
    const auto s = speed();
    const auto m = translation.magnitude();
    translation = translation * s / m;
    physicsBody_->body()->SetLinearVelocity(translation.toVec());
  }
}

void PlayerController::onLateUpdate() noexcept {
  Component::onLateUpdate();
  gameObject().lock()->transform().lock()->position() =
      physicsBody_->data().toVector2<float>();
}

#if !NDEBUG
void PlayerController::onRender() noexcept {
  Component::onRender();

  if (Input::keyDown(KeyboardKey::SPACE)) {
    const auto& transform = gameObject().lock()->transform().lock();
    const auto& pp = transform->position() + transform->scale() / 2.f;
    const auto& mp = Input::mousePosition();
    SDL_SetRenderDrawColor(Game::renderer(), 0, 255, 255, 100);
    SDL_RenderDrawLine(Game::renderer(), static_cast<int32_t>(pp.x()),
                       static_cast<int32_t>(pp.y()), mp.x(), mp.y());
  }
}
#endif