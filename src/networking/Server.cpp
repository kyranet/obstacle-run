// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "networking/Server.h"

#include <iostream>
#include <thread>
#include <utility>

#include "utils/DebugAssert.h"

Server::ServerClient::ServerClient(std::weak_ptr<Server> server, int socket,
                                   sockaddr addr) noexcept
    : server_(std::move(server)), socket_(socket), addr_(addr) {
  // Get the client's IP and port number
  socklen_t client_len = sizeof(struct sockaddr);
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  getnameinfo(&addr_, client_len, host, NI_MAXHOST, service, NI_MAXSERV,
              NI_NUMERICHOST | NI_NUMERICSERV);

  // Print out the clients IP and port number
  printf("[CLIENT] Received a connection from %s:%s\n", host, service);

  if (sendIdentify()) {
    printf("[CLIENT] Accepted a connection from %s:%s\n", host, service);

    status_ = ClientStatus::kRunning;
    pushEvent({ClientEvent::kConnect, this,
               new client_event_connect_t{host, service}});
  }
}

Server::ServerClient::~ServerClient() noexcept {
  socket_close(socket_);
  status_ = ClientStatus::kClosed;
}

void Server::ServerClient::run() noexcept {
  std::cout << "[CLIENT] Running.\n";

  while (running()) {
    debug_print("[CLIENT] Waiting message from: %i\n", id());

    // Read the buffer from the client
    uint8_t message[64];
    ssize_t len = socket_read(socket_, message, 64);
    if (len <= 0) {
      if (len == 0)
        std::cout << "[CLIENT] Disconnected.\n";
      else
        std::cerr << "[CLIENT] Encountered an error.\n";

      disconnect();
      break;
    }

    // Messages must have at least 5 bytes (event_id {4}, player_id {1}):
    if (len <= 5) continue;

    // Print the received message
    debug_print("[CLIENT] Received [%i]: %.*s\n", len, len, message);
    parseMessage(message);
  }
}

void Server::ServerClient::parseMessage(uint8_t* message) noexcept {
  // Validate event number, if it does not match, skip:
  const auto counter = buffer_->readUInt32(message, 0);
  if (counter != remoteEvent_) return;

  // Increase the remote event's number:
  ++remoteEvent_;

  // Read the message type:
  const auto type =
      static_cast<IncomingMessageType>(buffer_->readUint8(message, 4));
  switch (type) {
    case IncomingMessageType::kUpdatePosition: {
      Vector2<float> position{buffer_->readFloat(message, 5),
                              buffer_->readFloat(message, 5 + sizeof(float))};
      pushEvent({ClientEvent::kUpdatePosition, this,
                 new client_event_player_update_t{position}});
      position_ = position;
      break;
    }
    case IncomingMessageType::kBulletShoot: {
      const auto angle = buffer_->readFloat(message, 5);
      pushEvent({ClientEvent::kBulletShoot, this,
                 new client_event_bullet_shoot_t{angle}});
      break;
    }
  }
}

bool Server::ServerClient::sendIdentify() noexcept {
  id_ = server_.lock()->nextPlayerID();
  uint8_t message[6];
  buffer_->writeUint8(
      message, static_cast<uint8_t>(OutgoingMessageType::kPlayerIdentify), 4);
  buffer_->writeUint8(message, id_, 5);
  return send(message, 6);
}

Server::Server() noexcept {
  status_ = ServerStatus::kPending;

  std::cout << "Starting server... ";

  server_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_ < 0) {
    std::cerr << "[SERVER] Failed to create socket.\n";
    exit(2);
  }

  struct sockaddr_in serv_addr;
  bzero((char*)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(9999);

  if (socket_bind(server_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <
      0) {
    std::cerr << "[SERVER] Failed to bind socket.\n";
    exit(2);
  }

  if (socket_listen(server_, 5) < 0) {
    std::cerr << "[SERVER] Failed to listen for new connections.\n";
    exit(2);
  }

  std::cout << "\033[0;32mReady!\033[0m\n";
  debug_print("[SERVER] FD [%i]\n", server_);
}

Server::~Server() noexcept {
  socket_close(server_);
  status_ = ServerStatus::kClosed;
}

void Server::handleEvents() noexcept {
  if (clients_.empty()) return;

  client_event_t event{};
  size_t i = clients_.size();
  while (i != 0) {
    auto& client = clients_[--i];
    while (client->readEvent(&event)) {
      if (event.event == ClientEvent::kDisconnect) {
        constexpr const auto size = 6;
        uint8_t message[size];
        buffer_->writeUint8(
            message,
            static_cast<uint8_t>(OutgoingMessageType::kPlayerDisconnect), 4);
        buffer_->writeUint8(message, client->id(), 5);
        broadcastExcept(message, 6, client->id());

        clients_.erase(clients_.begin() + i);
        break;
      }

      if (event.event == ClientEvent::kConnect) {
        {
          constexpr const auto size = 6;
          uint8_t message[size];
          buffer_->writeUint8(
              message,
              static_cast<uint8_t>(OutgoingMessageType::kPlayerConnect), 4);
          buffer_->writeUint8(message, client->id(), 5);
          broadcastExcept(message, size, client->id());
        }

        for (const auto& c : clients_) {
          // Do not send the same ID twice
          if (c->id() == client->id()) continue;

          constexpr const auto size = 6 + sizeof(float) * 2;
          uint8_t message[size];
          buffer_->writeUint8(
              message,
              static_cast<uint8_t>(OutgoingMessageType::kPlayerInsertPosition),
              4);
          buffer_->writeUint8(message, c->id(), 5);
          buffer_->writeFloat(message, c->position().x(), 6);
          buffer_->writeFloat(message, c->position().y(), 6 + sizeof(float));
          client->send(message, size);
        }
      } else if (event.event == ClientEvent::kUpdatePosition) {
        constexpr const auto size = 6 + sizeof(float) * 2;
        const auto* data =
            reinterpret_cast<client_event_player_update_t*>(event.data);
        uint8_t message[size];
        buffer_->writeUint8(
            message,
            static_cast<uint8_t>(OutgoingMessageType::kPlayerUpdatePosition),
            4);
        buffer_->writeUint8(message, client->id(), 5);
        buffer_->writeFloat(message, data->position_.x(), 6);
        buffer_->writeFloat(message, data->position_.y(), 6 + sizeof(float));
        broadcastExcept(message, size, client->id());
      } else if (event.event == ClientEvent::kBulletShoot) {
        constexpr const auto size = 5 + sizeof(float) * 2 + sizeof(double) * 2;
        const auto& data =
            *reinterpret_cast<client_event_bullet_shoot_t*>(event.data);

        const auto& sp = Vector2{50.f, 50.f} / 2.f;
        const auto& pp = event.client->position();

        const auto start =
            Vector2<float>(pp.x() - (sp.x() * cos(data.angle_) * 2.f),
                           pp.y() - (sp.y() * sin(data.angle_) * 2.f));
        const auto velocity = Vector2<double>{-cos(data.angle_) * 5000000000.0,
                                              -sin(data.angle_) * 5000000000.0};

        uint8_t message[size];
        buffer_->writeUint8(
            message, static_cast<uint8_t>(OutgoingMessageType::kBulletShoot),
            4);
        buffer_->writeFloat(message, start.x(), 5);
        buffer_->writeFloat(message, start.y(), 5 + sizeof(float));
        buffer_->writeDouble(message, velocity.x(), 5 + sizeof(float) * 2);
        buffer_->writeDouble(message, velocity.y(),
                             5 + sizeof(float) * 2 + sizeof(double));
        broadcast(message, size);
      }
    }
  }
}

void Server::run() noexcept {
  const constexpr static int64_t gameFrameRate = 60;
  const constexpr static auto frameTime =
      std::chrono::milliseconds(static_cast<int64_t>(1000 / gameFrameRate));

  std::cout << "[SERVER] Running.\n";
  status_ = ServerStatus::kRunning;

  std::thread([&]() {
    while (status_ == ServerStatus::kRunning) {
      struct sockaddr_in client_addr;
      socklen_t client_len = sizeof(client_addr);
      auto* casted_client_addr = (struct sockaddr*)&client_addr;

      int sd_client = accept(server_, casted_client_addr, &client_len);
      if (sd_client < 0) {
        std::cerr << "[CLIENT] Errored while accepting a connection.\n";
        exit(1);
      }

      debug_print("[CLIENT] FD [%i]\n", sd_client);

      auto sc =
          new ServerClient(shared_from_this(), sd_client, *casted_client_addr);
      std::lock_guard<std::mutex> guard(pending_clients_mutex_);
      pending_clients_.push(sc);
    }
  }).detach();

  while (running()) {
    handleEvents();

    {
      std::lock_guard<std::mutex> guard(pending_clients_mutex_);
      while (!pending_clients_.empty()) {
        auto* client = pending_clients_.back();
        debug_print("[CLIENT] Inserting player: %d\n", client->id());
        pending_clients_.pop();
        std::thread([&]() { client->run(); }).detach();
        clients_.emplace_back(client);
      }
    }

    std::this_thread::sleep_for(frameTime);
  }
}
