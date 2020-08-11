// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PhysicsBody.h"

#include "Game.h"
#include "components/Transform.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"

PhysicsBody::PhysicsBody(std::weak_ptr<GameObject> parent, b2BodyType type,
                         bool sensor, Vector4<int32_t> data, uint16_t category,
                         uint16_t mask) noexcept
    : Component(std::move(parent)),
      type_(type),
      sensor_(sensor),
      data_(data),
      category_(category),
      mask_(mask) {
  b2BodyDef bodyDef;
  bodyDef.type = type_;
  bodyDef.position.Set(static_cast<float>(data.x()),
                       static_cast<float>(data.y()));
  bodyDef.angle = 0.f;
  bodyDef.fixedRotation = true;

  body_ = scene().lock()->world().CreateBody(&bodyDef);

  b2PolygonShape boxShape;
  boxShape.SetAsBox(static_cast<float>(data.z()) / 2.f,
                    static_cast<float>(data.a()) / 2.f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &boxShape;
  fixtureDef.density = 1.f;
  fixtureDef.userData = this;
  fixtureDef.isSensor = sensor_;
  fixtureDef.filter.categoryBits = category_;
  fixtureDef.filter.maskBits = mask_;
  body_->CreateFixture(&fixtureDef);
}

PhysicsBody::~PhysicsBody() noexcept = default;

void PhysicsBody::onLateUpdate() noexcept {
  Component::onLateUpdate();

  data_.x() = static_cast<int32_t>(body()->GetPosition().x - data().z() / 2.f);
  data_.y() = static_cast<int32_t>(body()->GetPosition().y - data().a() / 2.f);
}

#if !NDEBUG
void PhysicsBody::onRender() noexcept {
  Component::onRender();

  const auto destination = rectangle();
  SDL_SetRenderDrawColor(Game::renderer(), 255, sensor() ? 128 : 0, 0, 100);
  SDL_RenderDrawRect(Game::renderer(), &destination);
}
#endif
