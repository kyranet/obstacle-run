// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PhysicsBody.h"

#include "Game.h"
#include "components/Transform.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"

PhysicsBody::PhysicsBody(std::weak_ptr<GameObject> parent) noexcept
    : Component(std::move(parent)) {}

PhysicsBody::~PhysicsBody() noexcept {
  if (gameObject().expired()) return;

  auto go = gameObject().lock();
  if (go->scene().expired()) return;

  auto& world = go->scene().lock()->world();
  if (!world.IsLocked()) world.DestroyBody(body());
}

void PhysicsBody::onAwake() noexcept {
  Component::onAwake();
  refresh();
}

void PhysicsBody::onLateUpdate() noexcept {
  Component::onLateUpdate();

  auto tf = gameObject().lock()->transform().lock();
  auto& ps = tf->position();
  auto& sc = tf->scale();

  ps.x() = body()->GetPosition().x - sc.x() / 2.f;
  ps.y() = body()->GetPosition().y - sc.y() / 2.f;
}

#if !NDEBUG
void PhysicsBody::onRender() noexcept {
  Component::onRender();

  auto tf = gameObject().lock()->transform().lock();
  auto& ps = tf->position();
  auto& sc = tf->scale();

  const auto destination =
      SDL_Rect{static_cast<int32_t>(ps.x()), static_cast<int32_t>(ps.y()),
               sc.x(), sc.y()};
  SDL_SetRenderDrawColor(Game::renderer(), 255, sensor() ? 128 : 0,
                         type() == b2BodyType::b2_dynamicBody ? 255 : 0, 100);
  SDL_RenderDrawRect(Game::renderer(), &destination);
}
#endif

Json::Value PhysicsBody::toJson() const noexcept {
  auto json = Component::toJson();
  json["name"] = "PhysicsBody";
  json["type"] = getNameFromBodyType(type());
  json["sensor"] = sensor();
  json["density"] = density();
  json["restitution"] = restitution();
  json["linear_damping"] = linearDamping();
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

void PhysicsBody::patch(const Json::Value& json) noexcept {
  patch(physics_body_patch_t{{json["id"].asUInt(), json["enabled"].asBool()},
                             getBodyTypeFromName(json["type"].asString()),
                             json["sensor"].asBool(),
                             json["density"].asFloat(),
                             json["restitution"].asFloat(),
                             json["linear_damping"].asFloat(),
                             getMaskFromJson(json["category"]),
                             getMaskFromJson(json["mask"])});
}

b2BodyType PhysicsBody::getBodyTypeFromName(const std::string& value) noexcept {
  if (value == "static") return b2BodyType::b2_staticBody;
  if (value == "kinematic") return b2BodyType::b2_kinematicBody;

  assert(((void)"'value' must be one of 'static', 'kinematic', or 'dynamic'.",
          value == "dynamic"));
  return b2BodyType::b2_dynamicBody;
}

PhysicsBodyMask PhysicsBody::getBodyMaskFromName(
    const std::string& value) noexcept {
  if (value == "boundary") return PhysicsBodyMask::Boundary;
  if (value == "player") return PhysicsBodyMask::Player;
  if (value == "enemy") return PhysicsBodyMask::Enemy;
  if (value == "collectible") return PhysicsBodyMask::Collectible;
  if (value == "bullet") return PhysicsBodyMask::Bullet;

  assert(((void)"'value' must be one of 'boundary', 'player', 'enemy', "
                "'collectible', 'bullet', or 'goal'.",
          value == "goal"));
  return PhysicsBodyMask::Goal;
}

uint16_t PhysicsBody::getMaskFromJson(const Json::Value& json) noexcept {
  assert(((void)"'json' must be an array.", json.isArray()));

  uint16_t bits = 0;
  for (const auto& mask : json) {
    assert(((void)"'mask' must be a string.", mask.isString()));
    bits |= static_cast<uint16_t>(getBodyMaskFromName(mask.asString()));
  }

  return bits;
}

void PhysicsBody::patch(const physics_body_patch_t& data) noexcept {
  Component::patch(data);

  type_ = data.type;
  sensor_ = data.sensor;
  density_ = data.density;
  restitution_ = data.restitution;
  linearDamping_ = data.linearDamping;
  category_ = data.category;
  mask_ = data.mask;
}

void PhysicsBody::refresh() noexcept {
  const auto tf = gameObject().lock()->transform().lock();
  const auto& ps = tf->position();
  const auto& sc = tf->scale();

  b2BodyDef bodyDef;
  bodyDef.type = type_;
  bodyDef.position.Set(static_cast<float>(ps.x()), static_cast<float>(ps.y()));
  bodyDef.angle = 0.f;
  bodyDef.fixedRotation = true;
  bodyDef.linearDamping = linearDamping_;
  bodyDef.awake = true;

  body_ = scene().lock()->world().CreateBody(&bodyDef);

  b2PolygonShape boxShape;
  boxShape.SetAsBox(static_cast<float>(sc.x()) / 2.f,
                    static_cast<float>(sc.y()) / 2.f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &boxShape;
  fixtureDef.density = density_;
  fixtureDef.isSensor = sensor_;
  fixtureDef.filter.categoryBits = category_;
  fixtureDef.filter.maskBits = mask_;
  fixtureDef.restitution = restitution_;
  fixtureDef.userData = this;
  body_->CreateFixture(&fixtureDef);
}
