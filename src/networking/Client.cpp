// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "networking/Client.h"

#include "utils/DebugAssert.h"

Client::Client() noexcept {
  std::cout << "Starting client... ";

  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, "localhost", 9999) == -1) {
    printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    exit(1);
  }

  socket_ = SDLNet_TCP_Open(&ip);
  if (!socket_) {
    std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << '\n';
    exit(2);
  }

  status_ = ClientStatus::kRunning;
  std::cout << "\033[0;32mReady!\033[0m\n";
}

Client::~Client() noexcept { SDLNet_TCP_Close(socket_); }

void Client::run() noexcept {
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
    debug_print("Received: %.*s\n", len, message);
    deserializeMessage(message);
  }
}

void Client::deserializeMessage(uint8_t* message) noexcept {
  // Validate event number, if it does not match, skip:
  const auto counter = buffer_->readUInt32(message, 0);
  if (counter != event_) return;

  // Read the message type:
  const auto type = static_cast<ClientEvent>(buffer_->readUint8(message, 4));
  switch (type) {
    case ClientEvent::kConnect: {
      const auto id = buffer_->readUint8(message, 5);
      pushEvent({type, new client_event_connect_t{id}});
      break;
    }
    case ClientEvent::kDisconnect: {
      const auto id = buffer_->readUint8(message, 5);
      pushEvent({type, new client_event_disconnect_t{id}});
      break;
    }
    case ClientEvent::kUpdatePosition: {
      const auto id = buffer_->readUint8(message, 5);
      Vector2<float> position{buffer_->readFloat(message, 6),
                              buffer_->readFloat(message, 6 + sizeof(float))};
      pushEvent({type, new client_event_player_update_t{id, position}});
      break;
    }
  }
}

std::tuple<uint8_t*, int32_t> Client::serializeMessage(
    ClientEvent event, void* data) const noexcept {
  switch (event) {
    case ClientEvent::kConnect:
    case ClientEvent::kDisconnect: {
      constexpr static int32_t size = 5;
      uint8_t message[size];
      buffer_->writeUint32(message, event_, 0);
      buffer_->writeUint8(message, static_cast<uint8_t>(event), 4);
      return {message, size};
    }
    case ClientEvent::kUpdatePosition: {
      constexpr static int32_t size = 5 + sizeof(float) * 2;
      const auto& vector = reinterpret_cast<Vector2<float>*>(data);
      uint8_t message[size];
      buffer_->writeUint32(message, event_, 0);
      buffer_->writeUint8(message, static_cast<uint8_t>(event), 4);
      buffer_->writeFloat(message, vector->x(), 5);
      buffer_->writeFloat(message, vector->y(), 5 + sizeof(float));
      return {message, size};
    }
  }
  return std::pair<uint8_t*, int32_t>();
}
