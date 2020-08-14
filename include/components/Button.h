// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"

class Button final : public Component {
 public:
  explicit Button(std::weak_ptr<GameObject> gameObject) noexcept;

  void onUpdate() noexcept override;

  [[nodiscard]] virtual Json::Value toJson() const noexcept override;
};
