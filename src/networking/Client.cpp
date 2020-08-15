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

  std::cout << "\033[0;32mReady!\033[0m\n";
}

Client::~Client() noexcept { SDLNet_TCP_Close(socket_); }

void Client::run() noexcept {
  status_ = ClientStatus::kRunning;
  std::cout << "[CLIENT] Running.\n";

  while (running()) {
    // Read the buffer from the client
    uint8_t message[64];
    int len = SDLNet_TCP_Recv(socket_, message, 64);
    if (len <= 0) {
      if (len == 0)
        std::cout << "[CLIENT] Disconnected.\n";
      else
        std::cerr << "[CLIENT] TCP Error: " << SDLNet_GetError() << '\n';
      status_ = ClientStatus::kClosed;
      break;
    }

    // Print the received message
    debug_print("[CLIENT] Received [%i]: %.*s\n", len, len, message);
    deserializeMessage(message);
  }

  status_ = ClientStatus::kPending;
}

void Client::deserializeMessage(uint8_t* message) noexcept {
  // Validate event number, if it does not match, skip:
  const auto counter = buffer_->readUInt32(message, 0);
  if (counter != remoteEvent_) return;

  // Increase the remote event's number:
  ++remoteEvent_;

  // Read the message type:
  const auto type =
      static_cast<IncomingClientEvent>(buffer_->readUint8(message, 4));
  switch (type) {
    case IncomingClientEvent::kPlayerIdentify: {
      const auto id = buffer_->readUint8(message, 5);
      pushEvent({type, new client_event_identify_t{id}});
      break;
    }
    case IncomingClientEvent::kPlayerConnect: {
      const auto id = buffer_->readUint8(message, 5);
      pushEvent({type, new client_event_connect_t{id}});
      break;
    }
    case IncomingClientEvent::kPlayerDisconnect: {
      const auto id = buffer_->readUint8(message, 5);
      pushEvent({type, new client_event_disconnect_t{id}});
      break;
    }
    case IncomingClientEvent::kPlayerUpdatePosition: {
      const auto id = buffer_->readUint8(message, 5);
      Vector2<float> position{buffer_->readFloat(message, 6),
                              buffer_->readFloat(message, 6 + sizeof(float))};
      pushEvent({type, new client_event_player_update_t{id, position}});
      break;
    }
  }
}

std::tuple<uint8_t*, int32_t> Client::serializeMessage(
    OutgoingClientEvent event, const void* data) const noexcept {
  switch (event) {
    case OutgoingClientEvent::kUpdatePosition: {
      constexpr static int32_t size = 5 + sizeof(float) * 2;
      const auto& vector = reinterpret_cast<const Vector2<float>*>(data);
      uint8_t message[size];
      buffer_->writeUint8(message, static_cast<uint8_t>(event), 4);
      buffer_->writeFloat(message, vector->x(), 5);
      buffer_->writeFloat(message, vector->y(), 5 + sizeof(float));
      return {message, size};
    }
  }

  return std::pair<uint8_t*, int32_t>();
}
