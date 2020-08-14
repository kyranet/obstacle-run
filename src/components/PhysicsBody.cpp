// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PhysicsBody.h"

#include "Game.h"
#include "components/Transform.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"

PhysicsBody::PhysicsBody(std::weak_ptr<GameObject> parent, b2BodyType type,
                         bool sensor, float density, float restitution,
                         float linearDamping, Vector4<int32_t> data,
                         uint16_t category, uint16_t mask) noexcept
    : Component(std::move(parent)),
      type_(type),
      sensor_(sensor),
      density_(density),
      restitution_(restitution),
      linearDamping_(linearDamping),
      data_(data),
      category_(category),
      mask_(mask) {
  b2BodyDef bodyDef;
  bodyDef.type = type_;
  bodyDef.position.Set(static_cast<float>(data.x()),
                       static_cast<float>(data.y()));
  bodyDef.angle = 0.f;
  bodyDef.fixedRotation = true;
  bodyDef.linearDamping = linearDamping;
  bodyDef.awake = true;

  body_ = scene().lock()->world().CreateBody(&bodyDef);

  b2PolygonShape boxShape;
  boxShape.SetAsBox(static_cast<float>(data.z()) / 2.f,
                    static_cast<float>(data.a()) / 2.f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &boxShape;
  fixtureDef.density = density;
  fixtureDef.isSensor = sensor_;
  fixtureDef.filter.categoryBits = category_;
  fixtureDef.filter.maskBits = mask_;
  fixtureDef.restitution = restitution;
  fixtureDef.userData = this;
  body_->CreateFixture(&fixtureDef);
}

PhysicsBody::~PhysicsBody() noexcept {
  if (gameObject().expired()) return;

  auto go = gameObject().lock();
  if (go->scene().expired()) return;

  auto& world = go->scene().lock()->world();
  if (!world.IsLocked()) world.DestroyBody(body());
}

void PhysicsBody::onLateUpdate() noexcept {
  Component::onLateUpdate();

  data_.x() = static_cast<int32_t>(body()->GetPosition().x - data().z() / 2.f);
  data_.y() = static_cast<int32_t>(body()->GetPosition().y - data().a() / 2.f);
}

#if !NDEBUG
void PhysicsBody::onRender() noexcept {
  Component::onRender();

  const auto destination = rectangle();
  SDL_SetRenderDrawColor(Game::renderer(), 255, sensor() ? 128 : 0,
                         type() == b2BodyType::b2_dynamicBody ? 255 : 0, 100);
  SDL_RenderDrawRect(Game::renderer(), &destination);
}
#endif

Json::Value PhysicsBody::toJson() const noexcept {
  Json::Value json(Json::objectValue);
  json["name"] = "PhysicsBody";
  json["type"] = getNameFromBodyType(type());
  json["sensor"] = sensor();
  json["density"] = density();
  json["restitution"] = restitution();
  json["linear_damping"] = linearDamping();
  json["data"] = data().toJson();
  json["category"] = getJsonFromMask(category());
  json["mask"] = getJsonFromMask(mask());
  return json;
}

std::string PhysicsBody::getNameFromBodyType(b2BodyType value) noexcept {
  switch (value) {
    case b2BodyType::b2_staticBody:
      return "static";
    case b2BodyType::b2_kinematicBody:
      return "kinematic";
    default:
      return "dynamic";
  }
}

Json::Value PhysicsBody::getJsonFromMask(const uint16_t bits) noexcept {
  Json::Value json(Json::arrayValue);
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Boundary))
    json.append("boundary");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Player))
    json.append("player");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Enemy))
    json.append("enemy");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Collectible))
    json.append("collectible");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Bullet))
    json.append("bullet");
  if (bits & static_cast<uint16_t>(PhysicsBodyMask::Goal)) json.append("goal");
  return json;
}
