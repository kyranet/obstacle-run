// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <box2d/box2d.h>

#include "objects/Component.h"
#include "utils/Vector4.h"

class PhysicsBody final : public Component {
  b2Body* body_{};
  b2BodyType type_;
  bool sensor_;
  float density_;
  float restitution_;
  float linearDamping_;
  Vector4<int32_t> data_;
  uint16_t category_;
  uint16_t mask_;

 public:
  PhysicsBody(std::weak_ptr<GameObject> gameObject, b2BodyType type,
              bool sensor, float density, float restitution,
              float linearDamping, Vector4<int32_t> vector, uint16_t category,
              uint16_t mask) noexcept;
  ~PhysicsBody() noexcept override;

  [[nodiscard]] inline const Vector4<int32_t>& data() const noexcept {
    return data_;
  }

  [[nodiscard]] inline b2Body* body() const noexcept { return body_; }

  [[nodiscard]] inline const b2BodyType& type() const noexcept { return type_; }

  [[nodiscard]] inline const bool& sensor() const noexcept { return sensor_; }

  [[nodiscard]] inline const float& density() const noexcept {
    return density_;
  }

  [[nodiscard]] inline const float& restitution() const noexcept {
    return restitution_;
  }

  [[nodiscard]] inline const float& linearDamping() const noexcept {
    return linearDamping_;
  }

  [[nodiscard]] inline const uint16_t& category() const noexcept {
    return category_;
  }

  [[nodiscard]] inline const uint16_t& mask() const noexcept { return mask_; }

  [[nodiscard]] inline SDL_Rect rectangle() const noexcept {
    return data().toRectangle();
  }

#if !NDEBUG
  void onRender() noexcept override;
#endif
  void onLateUpdate() noexcept override;
};
