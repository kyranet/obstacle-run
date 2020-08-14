// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/NetworkControllerFactory.h"

NetworkControllerFactory::NetworkControllerFactory() noexcept
    : ComponentFactory("NetworkController") {}
NetworkControllerFactory::~NetworkControllerFactory() noexcept = default;

// {
//   "name": "NetworkController"
// }

std::shared_ptr<NetworkController> NetworkControllerFactory::fromJson(
    const Json::Value&, std::weak_ptr<GameObject> parent) const noexcept {
  return std::make_shared<NetworkController>(parent);
}
