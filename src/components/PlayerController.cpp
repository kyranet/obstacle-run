// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PlayerController.h"

#include <utility>

#if !NDEBUG
#include "Game.h"
#endif
#include "components/BulletBox.h"
#include "components/PhysicsBody.h"
#include "components/Transform.h"
#include "managers/Input.h"
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
    const auto start = Vector2<float>(pp.x() - (sp.x() * cos(angle) * 2.f),
                                      pp.y() - (sp.y() * sin(angle) * 2.f));
    const auto bs = Vector2<int32_t>{8, 8};
    const auto bp = Vector4<int32_t>{start, bs};
    const auto go = std::make_shared<GameObject>(scene());
    go->name() = "Bullet";
    go->active() = true;

    const auto newTransform =
        std::make_shared<Transform>(go->shared_from_this());
    newTransform->patch({{0, true}, bp.toVector2<float>(), bs});

    const auto newPhysics =
        std::make_shared<PhysicsBody>(go->shared_from_this());
    newPhysics->patch({{1, true},
                       b2BodyType::b2_dynamicBody,
                       false,
                       1.f,
                       1.f,
                       0.4f,
                       static_cast<uint16_t>(1 << 4),
                       static_cast<uint16_t>(0b11111)});

    const auto newBullet = std::make_shared<BulletBox>(go->shared_from_this());
    newBullet->patch({{2, true},
                      2.f,
                      bp,
                      Vector2<double>{-cos(angle) * 5000000000.0,
                                      -sin(angle) * 5000000000.0}});

    go->addComponent(newTransform);
    go->addComponent(newPhysics);
    go->addComponent(newBullet);
    go->onAwake();
    bullets_.lock()->addChild(go);
  }
}

void PlayerController::onLateUpdate() noexcept { Component::onLateUpdate(); }

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
