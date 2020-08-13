// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/NetworkController.h"
#include "interfaces/ComponentFactory.h"

class NetworkControllerFactory final
    : public ComponentFactory<NetworkController> {
 public:
  NetworkControllerFactory() noexcept;
  ~NetworkControllerFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<NetworkController> fromJson(
      const Json::Value& json, std::weak_ptr<GameObject> parent) override;
  [[nodiscard]] Json::Value toJson(
      std::shared_ptr<NetworkController> value) const override;
};
