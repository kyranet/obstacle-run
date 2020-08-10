// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PhysicsBody.h"

#include "Game.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"

PhysicsBody::PhysicsBody(std::weak_ptr<GameObject> parent,
                         Vector4<int32_t> data) noexcept
    : Component(std::move(parent)), data_(data) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.position.Set(static_cast<float>(data.x()),
                       static_cast<float>(data.y()));
  bodyDef.angle = 0.f;

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

#if !NDEBUG
void PhysicsBody::onRender() noexcept {
  Component::onRender();

  const auto destination = data().toRectangle();
  SDL_SetRenderDrawColor(Game::renderer(), 255, 0, 0, 100);
  SDL_RenderDrawRect(Game::renderer(), &destination);
}
#endif
