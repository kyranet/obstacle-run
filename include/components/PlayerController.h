// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"

class Transform;
class PhysicsBody;
class NetworkController;

class PlayerController final : public Component {
  double bulletNext_{};
  uint8_t bulletClip_{};
  uint8_t speed_{};
  std::shared_ptr<PhysicsBody> physicsBody_{};
  std::weak_ptr<GameObject> bullets_{};
  std::weak_ptr<NetworkController> network_{};

 public:
  explicit PlayerController(std::weak_ptr<GameObject> gameObject) noexcept;
  ~PlayerController() noexcept override;
  void onAwake() noexcept override;
  void onUpdate() noexcept override;
  void onLateUpdate() noexcept override;
#if !NDEBUG
  void onRender() noexcept override;
#endif

  [[nodiscard]] inline const double& bulletNext() const noexcept {
    return bulletNext_;
  }

  [[nodiscard]] inline const uint8_t& speed() const noexcept { return speed_; }
  inline uint8_t& speed() noexcept { return speed_; }

  [[nodiscard]] inline const uint8_t& bulletClip() const noexcept {
    return bulletClip_;
  }
  inline uint8_t& bulletClip() noexcept { return bulletClip_; }

  [[nodiscard]] Json::Value toJson() const noexcept override;
  void patch(const Json::Value& json) noexcept override;
};
