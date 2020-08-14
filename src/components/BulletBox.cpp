// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/BulletBox.h"

#include <utility>

#include "components/PhysicsBody.h"
#include "components/Transform.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"
#include "utils/Time.h"

BulletBox::BulletBox(std::weak_ptr<GameObject> gameObject,
                     Vector4<int32_t> data, Vector2<double> velocity,
                     double remaining) noexcept
    : Component(std::move(gameObject)),
      remaining_(remaining),
      data_(std::move(data)),
      velocity_(std::move(velocity)) {}

void BulletBox::onAwake() noexcept {
  Component::onAwake();

  auto go = gameObject().lock();
  body_ = go->getComponent<PhysicsBody>();

  auto* pBody = body_.lock()->body();
  pBody->SetTransform(data_.toVector2<float>().toVec(), 0.f);
  pBody->ApplyForceToCenter(velocity_.toVec(), true);
}

void BulletBox::onUpdate() noexcept {
  Component::onUpdate();

  remaining_ -= Time::delta();
  if (remaining_ <= 0.0) {
    // Delete temporary physics body and this component
    body_.lock()->destroy();
    destroy();

    auto walls =
        scene().lock()->getGameObjectByName("Destructible-Walls").lock();
    const auto newPhysicsBody = std::make_shared<PhysicsBody>(
        walls->shared_from_this(), b2BodyType::b2_dynamicBody, false, 1000000.f,
        0.f, 1.f,
        Vector4<int32_t>{body_.lock()->data().toVector2<int32_t>(),
                         Vector2<int32_t>{50, 50}},
        static_cast<uint16_t>(0b00001), static_cast<uint16_t>(0b10011));
    newPhysicsBody->onAwake();
    walls->addComponent(newPhysicsBody);
  }
}

Json::Value BulletBox::toJson() const noexcept {
  auto json = Component::toJson();
  json["name"] = "BulletBox";
  json["data"] = data().toJson();
  json["velocity"] = velocity().toJson();
  json["remaining"] = remaining();
  return json;
}
