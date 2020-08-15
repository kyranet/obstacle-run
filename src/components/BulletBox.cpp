// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/BulletBox.h"

#include <utility>

#include "components/PhysicsBody.h"
#include "components/Transform.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"
#include "utils/Time.h"

BulletBox::BulletBox(std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)) {}

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
    auto go = std::make_shared<GameObject>(scene(), walls->shared_from_this());
    go->name() = "Wall";
    go->active() = true;

    const auto newTransform =
        std::make_shared<Transform>(go->shared_from_this());
    newTransform->patch({{0, true},
                         gameObject().lock()->transform().lock()->position(),
                         {50, 50}});
    const auto newPhysicsBody =
        std::make_shared<PhysicsBody>(go->shared_from_this());
    newPhysicsBody->patch({{1, true},
                           b2BodyType::b2_dynamicBody,
                           false,
                           1000000.f,
                           0.f,
                           1.f,
                           static_cast<uint16_t>(0b00001),
                           static_cast<uint16_t>(0b10011)});
    go->addComponent(newTransform);
    go->addComponent(newPhysicsBody);
    go->onAwake();
    walls->addChild(go);
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

void BulletBox::patch(const Json::Value& json) noexcept {
  patch(bullet_box_patch_t{{json["id"].asUInt(), json["enabled"].asBool()},
                           json["remaining"].asDouble(),
                           Vector4<int32_t>(json["data"]),
                           Vector2<double>(json["velocity"])});
}

void BulletBox::patch(const bullet_box_patch_t& json) noexcept {
  Component::patch(json);
  data_ = json.data;
  velocity_ = json.velocity;
  remaining_ = json.remaining;
}
