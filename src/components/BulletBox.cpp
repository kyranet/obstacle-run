// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/BulletBox.h"

#include <utility>

#include "components/PhysicsBody.h"
#include "components/Transform.h"
#include "objects/GameObject.h"
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
  }
}
