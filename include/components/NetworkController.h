// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"

class Client;

class NetworkController final : public Component {
  std::unique_ptr<Client> client_;
  std::weak_ptr<GameObject> players_;

  void createPlayer(uint8_t id);
  void removePlayer(uint8_t id);

 public:
  explicit NetworkController(std::weak_ptr<GameObject> gameObject) noexcept;
  ~NetworkController() noexcept override;

  void onAwake() noexcept override;
  void onUpdate() noexcept override;

  [[nodiscard]] const std::unique_ptr<Client>& client() const noexcept {
    return client_;
  }

  [[nodiscard]] Json::Value toJson() const noexcept override;
};
