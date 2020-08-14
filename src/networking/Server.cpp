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
  printf("Accepted a connection from %d.%d.%d.%d port %hu\n", ipAddress >> 24u,
         (ipAddress >> 16u) & 0xFFu, (ipAddress >> 8u) & 0xFFu,
         ipAddress & 0xFFu, remoteIP->port);

  if (sendIdentify()) {
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
  while (running()) {
    // Read the buffer from the client
    uint8_t message[1024];
    int len = SDLNet_TCP_Recv(socket_, message, 1024);
    if (!len) {
      std::cerr << "Received empty TCP message. Error: " << SDLNet_GetError()
                << '\n';
      break;
    }

    // Print the received message
    debug_print("[SERVER] Received: %.*s\n", len, message);
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

  status_ = ServerStatus::kRunning;
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
        clients_.erase(clients_.begin() + i);
        break;
      } else if (event.event == ClientEvent::kConnect) {
        uint8_t message[6];
        message[4] = static_cast<uint8_t>(OutgoingMessageType::kPlayerConnect);
        message[5] = client->id();
        broadcastExcept(message, 6, client->id());
      }
    }
  }
}

void Server::run() noexcept {
  const constexpr static uint32_t gameFrameRate = 60U;
  const constexpr static auto frameTime =
      static_cast<uint32_t>(1000 / gameFrameRate);

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
      clients_.emplace_back(
          std::make_unique<ServerClient>(shared_from_this(), client));
    }).detach();
  }
}
