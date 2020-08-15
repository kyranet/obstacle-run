// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/NetworkController.h"

#include <utils/DebugAssert.h>

#include <thread>
#include <utility>

#include "components/PhysicsBody.h"
#include "components/SolidRenderer.h"
#include "components/Transform.h"
#include "networking/Client.h"
#include "scenes/Scene.h"

NetworkController::NetworkController(
    std::weak_ptr<GameObject> gameObject) noexcept
    : Component(std::move(gameObject)) {}

NetworkController::~NetworkController() noexcept {
  if (enabled()) client()->disconnect();
}

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
    debug_print("[NETWORK] Received event %i\n", event.event);
    switch (event.event) {
      case IncomingClientEvent::kPlayerIdentify: {
        const auto* pack =
            reinterpret_cast<client_event_identify_t*>(event.data);
        players_.lock()->children()[0]->id() = pack->id_;
        break;
      }
      case IncomingClientEvent::kPlayerConnect: {
        const auto* pack =
            reinterpret_cast<client_event_connect_t*>(event.data);
        createPlayer(pack->player_);
        break;
      }
      case IncomingClientEvent::kPlayerDisconnect: {
        const auto* pack =
            reinterpret_cast<client_event_disconnect_t*>(event.data);
        removePlayer(pack->player_);
        break;
      }
      case IncomingClientEvent::kPlayerUpdatePosition: {
        const auto* pack =
            reinterpret_cast<client_event_player_update_t*>(event.data);
        movePlayer(pack->player_, pack->position_);
        break;
      }
    }
  }
}

Json::Value NetworkController::toJson() const noexcept {
  auto json = Component::toJson();
  json["name"] = "NetworkController";
  return json;
}

void NetworkController::createPlayer(uint8_t id) const noexcept {
  auto go = std::make_shared<GameObject>(scene(), players_);
  go->name() = "Opponent";
  go->active() = true;
  go->id() = id;

  auto newTransform = std::make_shared<Transform>(go->shared_from_this());
  newTransform->patch({{0, true}, {50.f, 50.f}, {50, 50}});

  constexpr const auto category = static_cast<uint16_t>(PhysicsBodyMask::Enemy);
  constexpr const auto mask =
      static_cast<uint16_t>(PhysicsBodyMask::Boundary) |
      static_cast<uint16_t>(PhysicsBodyMask::Enemy) |
      static_cast<uint16_t>(PhysicsBodyMask::Collectible) |
      static_cast<uint16_t>(PhysicsBodyMask::Bullet) |
      static_cast<uint16_t>(PhysicsBodyMask::Player);
  auto newPhysics = std::make_shared<PhysicsBody>(go->shared_from_this());
  newPhysics->patch({{1, true},
                     b2BodyType::b2_dynamicBody,
                     false,
                     10000,
                     0,
                     1,
                     category,
                     mask});

  auto newSolid = std::make_shared<SolidRenderer>(go->shared_from_this());
  newSolid->patch({{2, true}, {0, 0, 50, 50}, {0, 128, 255, 255}});

  go->addComponent(newTransform);
  go->addComponent(newPhysics);
  go->addComponent(newSolid);
  go->onAwake();
  players_.lock()->addChild(go);
}

void NetworkController::removePlayer(uint8_t id) const noexcept {
  for (const auto& player : players_.lock()->children()) {
    if (player->id() == id) {
      player->destroy();
      break;
    }
  }
}

void NetworkController::movePlayer(
    uint8_t id, const Vector2<float>& position) const noexcept {
  for (const auto& player : players_.lock()->children()) {
    if (player->id() == id) {
      player->physics().lock()->body()->SetTransform(position.toVec(), 0.f);
      player->transform().lock()->position().set(position);
      break;
    }
  }
}
