// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PhysicsBody.h"

#include "Game.h"
#include "components/Transform.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"

PhysicsBody::PhysicsBody(std::weak_ptr<GameObject> parent, b2BodyType type,
                         Vector4<int32_t> data) noexcept
    : Component(std::move(parent)), type_(type), data_(data) {
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
  SDL_SetRenderDrawColor(Game::renderer(), 255, 0, 0, 100);
  SDL_RenderDrawRect(Game::renderer(), &destination);
}
#endif
