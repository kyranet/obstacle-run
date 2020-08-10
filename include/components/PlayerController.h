// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"
class Transform;
class PlayerController final : public Component {
  uint8_t speed_;

 public:
  PlayerController(std::weak_ptr<GameObject> gameObject,
                   uint8_t speed) noexcept;
  ~PlayerController() noexcept;
  void onAwake() noexcept override;
  void onUpdate() noexcept override;

  [[nodiscard]] inline const uint8_t& speed() const noexcept { return speed_; }
  inline uint8_t& speed() noexcept { return speed_; }
};
