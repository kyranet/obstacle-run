// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/NetworkController.h"

#include <utils/DebugAssert.h>

#include <thread>
#include <utility>

#include "networking/Client.h"
#include "scenes/Scene.h"

NetworkController::NetworkController(
    std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)), client_(nullptr) {}

NetworkController::~NetworkController() noexcept { client()->disconnect(); }

void NetworkController::onAwake() noexcept {
  Component::onAwake();

  players_ = scene().lock()->getGameObjectByName("Players");

  client_ = std::make_unique<Client>();
  std::thread([&]() {
    client_->run();
    destroy();
  }).detach();
}

void NetworkController::onUpdate() noexcept {
  Component::onUpdate();

  client_event_t event{};
  while (client_->readEvent(&event)) {
    debug_print("Received event %i", event.event);
  }
}
