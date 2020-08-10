// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <box2d/box2d.h>

#include "objects/Component.h"
#include "utils/Vector4.h"

class PhysicsBody final : public Component {
  b2Body* body_{};
  Vector4<int32_t> data_;

 public:
  PhysicsBody(std::weak_ptr<GameObject> gameObject,
              Vector4<int32_t> vector) noexcept;
  ~PhysicsBody() noexcept override;

  [[nodiscard]] inline const Vector4<int32_t>& data() const noexcept {
    return data_;
  }

#if !NDEBUG
  void onRender() noexcept override;
#endif
};
