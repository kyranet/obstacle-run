// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"
#include "utils/Vector4.h"

class PhysicsBody;
class BulletBox final : public Component {
  double remaining_;
  Vector4<int32_t> data_;
  Vector2<double> velocity_;
  std::weak_ptr<PhysicsBody> body_{};

 public:
  BulletBox(std::weak_ptr<GameObject> gameObject, Vector4<int32_t> data,
            Vector2<double> velocity, double remaining) noexcept;

  void onAwake() noexcept override;
  void onUpdate() noexcept override;

  [[nodiscard]] inline const double& remaining() const noexcept {
    return remaining_;
  }

  [[nodiscard]] inline const Vector4<int32_t>& data() const noexcept {
    return data_;
  }

  [[nodiscard]] inline const Vector2<double>& velocity() const noexcept {
    return velocity_;
  }

  [[nodiscard]] virtual Json::Value toJson() const noexcept override;
};
