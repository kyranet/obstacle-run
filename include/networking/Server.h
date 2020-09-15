// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <mutex>
#include <queue>
#include <shared_mutex>
#include <utility>

#include "utils/Buffer.h"
#include "utils/DebugAssert.h"
#include "utils/Vector2.h"

const auto socket_bind = &bind;
const auto socket_close = &close;
const auto socket_read = &read;
const auto socket_write = &write;
const auto socket_listen = &listen;

class Server : public std::enable_shared_from_this<Server> {
  enum class ClientStatus : uint8_t { kPending, kRunning, kClosed };
  enum class ClientEvent : uint8_t {
    kConnect,
    kDisconnect,
    kUpdatePosition,
    kBulletShoot
  };
  enum class OutgoingMessageType : uint8_t {
    kPlayerIdentify,
    kPlayerConnect,
    kPlayerDisconnect,
    kPlayerInsertPosition,
    kPlayerUpdatePosition,
    kBulletShoot
  };
  enum class IncomingMessageType : uint8_t { kUpdatePosition, kBulletShoot };
  class ServerClient;

  struct client_event_base_t {};

  struct client_event_connect_t : public client_event_base_t {
    explicit client_event_connect_t(char host[NI_MAXHOST],
                                    char service[NI_MAXSERV])
        : host_(host), service_(service) {}
    char* host_;
    char* service_;
  };

  struct client_event_disconnect_t : public client_event_base_t {};

  struct client_event_player_update_t : public client_event_base_t {
    explicit client_event_player_update_t(Vector2<float> position)
        : position_(std::move(position)) {}
    Vector2<float> position_;
  };

  struct client_event_bullet_shoot_t : public client_event_base_t {
    explicit client_event_bullet_shoot_t(float angle) : angle_(angle) {}
    float angle_;
  };

  struct client_event_t {
    ClientEvent event;
    ServerClient* client;
    client_event_base_t* data;
  };

  class ServerClient {
    ClientStatus status_ = ClientStatus::kPending;
    std::unique_ptr<Buffer> buffer_{};
    std::queue<client_event_t> events_{};
    std::mutex event_mutex_{};
    std::weak_ptr<Server> server_;
    int socket_;
    sockaddr addr_;
    uint8_t id_{0};
    uint32_t event_{0};
    uint32_t remoteEvent_{0};
    Vector2<float> position_{};

    void parseMessage(uint8_t* message) noexcept;
    bool sendIdentify() noexcept;

    inline void pushEvent(const client_event_t& event) noexcept {
      std::lock_guard<std::mutex> guard(event_mutex_);
      events_.push(event);
    }

   public:
    ServerClient(std::weak_ptr<Server> server, int socket,
                 sockaddr addr) noexcept;
    ~ServerClient() noexcept;

    void run() noexcept;

    [[nodiscard]] inline bool running() const noexcept {
      return status_ == ClientStatus::kRunning;
    }

    [[nodiscard]] inline const uint8_t& id() const noexcept { return id_; }

    [[nodiscard]] inline const Vector2<float>& position() const noexcept {
      return position_;
    }

    inline void disconnect() noexcept {
      pushEvent(
          {ClientEvent::kDisconnect, this, new client_event_disconnect_t{}});
      status_ = ClientStatus::kClosed;
    }

    inline bool send(uint8_t* data, int32_t size) noexcept {
      // Set the event count before sending:
      buffer_->writeUint32(data, event_, 0);

      debug_print("[CLIENT] Sending [%i]: %.*s\n", size, size, data);
      if (socket_write(socket_, data, size) < 0) {
        // Not all bits were sent, meaning an abrupt disconnection or unknown
        // socket error.
        disconnect();
        return false;
      }

      ++event_;
      return true;
    }

    inline bool readEvent(client_event_t* event) noexcept {
      std::lock_guard<std::mutex> guard(event_mutex_);
      if (events_.empty()) return false;

      *event = events_.front();
      events_.pop();
      return true;
    }
  };

  enum class ServerStatus : uint8_t { kPending, kRunning, kClosed };

  std::mutex pending_clients_mutex_{};
  std::queue<ServerClient*> pending_clients_{};

  std::vector<std::unique_ptr<ServerClient>> clients_{};
  std::unique_ptr<Buffer> buffer_{};
  std::mutex player_counter_mutex_{};
  uint8_t playerCounter_{0};
  ServerStatus status_;
  int server_;

  inline void broadcastExcept(uint8_t* data, int32_t size,
                              uint8_t id) noexcept {
    for (auto& client : clients_) {
      if (client->id() != id) client->send(data, size);
    }
  }

  inline void broadcast(uint8_t* data, int32_t size) noexcept {
    for (auto& client : clients_) {
      client->send(data, size);
    }
  }

  void handleEvents() noexcept;

 public:
  Server() noexcept;
  ~Server() noexcept;
  void run() noexcept;

  [[nodiscard]] inline bool running() const noexcept {
    return status_ == ServerStatus::kRunning;
  }

  [[nodiscard]] inline uint8_t nextPlayerID() noexcept {
    std::lock_guard<std::mutex> guard(player_counter_mutex_);
    return playerCounter_++;
  }
};
