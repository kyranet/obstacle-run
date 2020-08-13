// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"

class Client;

class NetworkController final : public Component {
  std::unique_ptr<Client> client_;
  std::weak_ptr<GameObject> players_;

 public:
  explicit NetworkController(std::weak_ptr<GameObject> gameObject) noexcept;
  ~NetworkController() noexcept override;

  void onAwake() noexcept override;
  void onUpdate() noexcept override;

  [[nodiscard]] const std::unique_ptr<Client>& client() const noexcept {
    return client_;
  }
};
