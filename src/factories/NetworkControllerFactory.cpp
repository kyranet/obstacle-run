// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/NetworkControllerFactory.h"

NetworkControllerFactory::NetworkControllerFactory() noexcept
    : ComponentFactory("NetworkController") {}
NetworkControllerFactory::~NetworkControllerFactory() noexcept = default;

// {
//   "name": "NetworkController"
// }

Json::Value NetworkControllerFactory::toJson(
    std::shared_ptr<NetworkController>) const {
  Json::Value json(Json::objectValue);
  json["name"] = name();
  return json;
}

std::shared_ptr<NetworkController> NetworkControllerFactory::fromJson(
    const Json::Value&, std::weak_ptr<GameObject> parent) {
  return std::make_shared<NetworkController>(parent);
}
