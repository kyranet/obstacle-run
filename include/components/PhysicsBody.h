// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"
#include "utils/Vector4.h"

class PhysicsBody final : public Component {
  uint32_t id_{};

 public:
  PhysicsBody(std::weak_ptr<GameObject> gameObject,
              Vector4<int32_t> vector) noexcept;
  ~PhysicsBody() noexcept;

  [[nodiscard]] const Vector4<int32_t>& data() const noexcept;

#if !NDEBUG
  void onRender() noexcept override;
#endif
};
