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
};
