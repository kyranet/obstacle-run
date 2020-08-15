// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL_net.h>

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>

#include "utils/Buffer.h"
#include "utils/Vector2.h"

enum class ClientStatus : uint8_t { kPending, kRunning, kClosed };
enum class IncomingClientEvent : uint8_t {
  kPlayerIdentify,
  kPlayerConnect,
  kPlayerDisconnect,
  kPlayerInsertPosition,
  kPlayerUpdatePosition,
  kBulletShoot
};
enum class OutgoingClientEvent : uint8_t { kUpdatePosition, kBulletShoot };

struct client_event_base_t {};

struct client_event_identify_t : public client_event_base_t {
  explicit client_event_identify_t(uint8_t id) : id_(id) {}
  uint8_t id_;
};

struct client_event_connect_t : public client_event_base_t {
  explicit client_event_connect_t(uint8_t player) : player_(player) {}
  uint8_t player_;
};

struct client_event_disconnect_t : public client_event_base_t {
  explicit client_event_disconnect_t(uint8_t player) : player_(player) {}
  uint8_t player_;
};

struct client_event_player_insert_t : public client_event_base_t {
  client_event_player_insert_t(uint8_t player, Vector2<float> position)
      : player_(player), position_(std::move(position)) {}
  uint8_t player_;
  Vector2<float> position_;
};

struct client_event_player_update_t : public client_event_base_t {
  client_event_player_update_t(uint8_t player, Vector2<float> position)
      : player_(player), position_(std::move(position)) {}
  uint8_t player_;
  Vector2<float> position_;
};

struct client_event_bullet_shoot_t : public client_event_base_t {
  client_event_bullet_shoot_t(Vector2<float> position, Vector2<double> velocity)
      : position_(std::move(position)), velocity_(std::move(velocity)) {}
  Vector2<float> position_;
  Vector2<double> velocity_;
};

struct client_event_t {
  IncomingClientEvent event;
  client_event_base_t* data;
};

class Client {
  ClientStatus status_ = ClientStatus::kPending;
  std::unique_ptr<Buffer> buffer_{};
  std::queue<client_event_t> events_{};
  std::mutex event_mutex_{};
  TCPsocket socket_;
  uint32_t remoteEvent_{0};
  uint32_t event_{0};
  uint8_t id_{0};

  void deserializeMessage(uint8_t* message) noexcept;

  inline void pushEvent(const client_event_t& event) noexcept {
    std::lock_guard<std::mutex> guard(event_mutex_);
    events_.push(event);
  }

 public:
  Client() noexcept;
  ~Client() noexcept;
  void run() noexcept;

  [[nodiscard]] inline const uint8_t& id() const noexcept { return id_; }

  [[nodiscard]] inline bool running() const noexcept {
    return status_ == ClientStatus::kRunning;
  }

  inline void disconnect() noexcept {
    pushEvent({IncomingClientEvent::kPlayerDisconnect,
               new client_event_disconnect_t{id_}});
    status_ = ClientStatus::kClosed;
  }

  inline void send(uint8_t* message, int32_t size) noexcept {
    buffer_->writeUint32(message, event_, 0);

    if (SDLNet_TCP_Send(socket_, message, size) != size) {
      // Not all bits were sent, meaning an abrupt disconnection or unknown
      // socket error.
      disconnect();
      return;
    }

    ++event_;
  }

  void send(OutgoingClientEvent event, const void* data = nullptr) noexcept;

  inline bool readEvent(client_event_t* event) noexcept {
    std::lock_guard<std::mutex> guard(event_mutex_);
    if (events_.empty()) return false;

    *event = events_.front();
    events_.pop();
    return true;
  }
};
