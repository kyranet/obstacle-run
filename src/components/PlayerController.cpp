// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PlayerController.h"

#include <utility>

#if !NDEBUG
#include "Game.h"
#endif
#include "components/BulletBox.h"
#include "components/NetworkController.h"
#include "components/PhysicsBody.h"
#include "components/Transform.h"
#include "managers/Input.h"
#include "networking/Client.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"
#include "utils/Time.h"

PlayerController::PlayerController(
    std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)) {}

PlayerController::~PlayerController() noexcept = default;

void PlayerController::onAwake() noexcept {
  Component::onAwake();

  auto go = gameObject().lock();
  physicsBody_ = go->getComponent<PhysicsBody>();
  bullets_ = go->scene().lock()->getGameObjectByName("Bullets");
  assert(((void)"'bullets_' must not be expired", !bullets_.expired()));

  network_ = go->scene()
                 .lock()
                 ->getGameObjectByName("Network")
                 .lock()
                 ->getComponent<NetworkController>();
}

void PlayerController::onUpdate() noexcept {
  Component::onUpdate();

  if (bulletNext_ != 0.0) {
    bulletNext_ -= Time::delta();
    if (bulletNext_ < 0.0) bulletNext_ = 0.0;
  }

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

  if (bulletNext_ == 0.0 && bulletClip_ && Input::keyDown(KeyboardKey::SPACE)) {
    --bulletClip_;
    bulletNext_ += 1.0;

    const auto transform = gameObject().lock()->transform().lock();

    const auto& sp = transform->scale().cast<float>() / 2.f;
    const auto& pp = transform->position() + sp;
    const auto& mp = Input::mousePosition();

    const auto angle = atan2(pp.y() - mp.y(), pp.x() - mp.x());
    network_.lock()->client()->send(OutgoingClientEvent::kBulletShoot, &angle);
  }
}

void PlayerController::onLateUpdate() noexcept {
  Component::onLateUpdate();

  const auto& vec =
      gameObject().lock()->physics().lock()->body()->GetPosition();
  auto position = Vector2<float>(vec);
  network_.lock()->client()->send(OutgoingClientEvent::kUpdatePosition,
                                  &position);
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

Json::Value PlayerController::toJson() const noexcept {
  auto json = Component::toJson();
  json["name"] = "PlayerController";
  json["bullet_clip"] = bulletClip();
  json["bullet_next"] = bulletNext();
  json["speed"] = speed();
  return json;
}

void PlayerController::patch(const Json::Value& json) noexcept {
  Component::patch(json);

  bulletNext_ = json["bullet_next"].asDouble();
  bulletClip_ = static_cast<uint8_t>(json["bullet_clip"].asUInt());
  speed_ = static_cast<uint8_t>(json["speed"].asUInt());
}
