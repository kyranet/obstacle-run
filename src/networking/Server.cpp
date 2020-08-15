// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "networking/Server.h"

#include <iostream>
#include <thread>
#include <utility>

#include "utils/DebugAssert.h"

Server::ServerClient::ServerClient(std::weak_ptr<Server> server,
                                   TCPsocket socket) noexcept
    : server_(std::move(server)), socket_(socket) {
  // Get the client's IP and port number
  const auto remoteIP = SDLNet_TCP_GetPeerAddress(socket_);
  if (!remoteIP) {
    printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
    return;
  }

  // Print out the clients IP and port number
  uint32_t ipAddress;
  ipAddress = SDL_SwapBE32(remoteIP->host);
  printf("[CLIENT] Received a connection from %d.%d.%d.%d port %hu\n",
         ipAddress >> 24u, (ipAddress >> 16u) & 0xFFu,
         (ipAddress >> 8u) & 0xFFu, ipAddress & 0xFFu, remoteIP->port);

  if (sendIdentify()) {
    printf("[CLIENT] Accepted a connection from %d.%d.%d.%d port %hu\n",
           ipAddress >> 24u, (ipAddress >> 16u) & 0xFFu,
           (ipAddress >> 8u) & 0xFFu, ipAddress & 0xFFu, remoteIP->port);

    status_ = ClientStatus::kRunning;
    pushEvent(
        {ClientEvent::kConnect, this, new client_event_connect_t{ipAddress}});
  }
}

Server::ServerClient::~ServerClient() noexcept {
  SDLNet_TCP_Close(socket_);
  status_ = ClientStatus::kClosed;
}

void Server::ServerClient::run() noexcept {
  std::cout << "[CLIENT] Running.\n";

  while (running()) {
    debug_print("[CLIENT] Waiting message from: %i\n", id());

    // Read the buffer from the client
    uint8_t message[64];
    int len = SDLNet_TCP_Recv(socket_, message, 64);
    if (len <= 0) {
      if (len == 0)
        std::cout << "[CLIENT] Disconnected.\n";
      else
        std::cerr << "[CLIENT] TCP Error: " << SDLNet_GetError() << '\n';

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

  if (SDL_Init(0) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    exit(1);
  }

  if (SDLNet_Init() == -1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    exit(2);
  }

  std::cout << "Starting server... ";

  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, nullptr, 9999) == -1) {
    std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << '\n';
    exit(1);
  }

  server_ = SDLNet_TCP_Open(&ip);
  if (!server_) {
    std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << '\n';
    exit(2);
  }

  std::cout << "\033[0;32mReady!\033[0m\n";
}

Server::~Server() noexcept {
  SDLNet_TCP_Close(server_);
  status_ = ServerStatus::kClosed;

  SDL_Quit();
  SDLNet_Quit();
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
  const constexpr static uint32_t gameFrameRate = 60U;
  const constexpr static auto frameTime =
      static_cast<uint32_t>(1000 / gameFrameRate);

  std::cout << "[SERVER] Running.\n";
  status_ = ServerStatus::kRunning;

  while (running()) {
    handleEvents();

    // Try to accept a connection
    auto* client = SDLNet_TCP_Accept(server_);

    // No connection accepted
    if (!client) {
      SDL_Delay(frameTime);
      continue;
    }

    std::thread([&, client]() {
      auto sc = new ServerClient(shared_from_this(), client);
      clients_.emplace_back(sc);
      sc->run();
    }).detach();
  }
}
