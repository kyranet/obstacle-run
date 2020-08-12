// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"

class Transform;
class PhysicsBody;

class PlayerController final : public Component {
  uint8_t bulletClip_;
  uint8_t speed_;
  std::shared_ptr<PhysicsBody> physicsBody_;
  std::weak_ptr<GameObject> bullets_;

 public:
  PlayerController(std::weak_ptr<GameObject> gameObject, uint8_t bulletClip,
                   uint8_t speed) noexcept;
  ~PlayerController() noexcept override;
  void onAwake() noexcept override;
  void onUpdate() noexcept override;
  void onLateUpdate() noexcept override;
#if !NDEBUG
  void onRender() noexcept override;
#endif

  [[nodiscard]] inline const uint8_t& speed() const noexcept { return speed_; }
  inline uint8_t& speed() noexcept { return speed_; }

  [[nodiscard]] inline const uint8_t& bulletClip() const noexcept {
    return bulletClip_;
  }
  inline uint8_t& bulletClip() noexcept { return bulletClip_; }
};
