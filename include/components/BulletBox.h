// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"
#include "utils/Vector4.h"

struct bullet_box_patch_t : component_patch_t {
  double remaining;
  Vector4<int32_t> data;
  Vector2<double> velocity;
};

class PhysicsBody;
class BulletBox final : public Component {
  double remaining_{};
  Vector4<int32_t> data_{};
  Vector2<double> velocity_{};
  std::weak_ptr<PhysicsBody> body_{};

 public:
  explicit BulletBox(std::weak_ptr<GameObject> gameObject) noexcept;

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

  [[nodiscard]] Json::Value toJson() const noexcept override;
  void patch(const Json::Value& json) noexcept override;
  void patch(const bullet_box_patch_t& json) noexcept;
};
