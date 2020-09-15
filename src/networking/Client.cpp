// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "networking/Client.h"

#include "utils/DebugAssert.h"

Client::Client() noexcept {
  std::cout << "[CLIENT] Starting client... ";

  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    std::cerr << "[CLIENT] Failed to open a socket.\n";
    exit(2);
  }

  auto* server = gethostbyname("0.0.0.0");
  if (server == nullptr) {
    std::cerr << "[CLIENT] There is no such host.\n";
    exit(2);
  }

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(9999);

  if (socket_connect(socket_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) <
      0) {
    std::cerr << "[CLIENT] Failed to connect to server.";
    exit(2);
  }

  std::cout << "\033[0;32mReady!\033[0m\n";
}

Client::~Client() noexcept { socket_close(socket_); }

void Client::run() noexcept {
  status_ = ClientStatus::kRunning;
  std::cout << "[CLIENT] Running.\n";

  while (running()) {
    // Read the buffer from the client
    uint8_t message[64];
    ssize_t len = socket_read(socket_, message, 64);
    if (len <= 0) {
      if (len == 0)
        std::cout << "[CLIENT] Disconnected.\n";
      else
        std::cerr << "[CLIENT] Encountered an error.\n";
      status_ = ClientStatus::kClosed;
      return;
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
    case IncomingClientEvent::kPlayerInsertPosition: {
      const auto id = buffer_->readUint8(message, 5);
      Vector2<float> position{buffer_->readFloat(message, 6),
                              buffer_->readFloat(message, 6 + sizeof(float))};
      pushEvent({type, new client_event_player_insert_t{id, position}});
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
    case IncomingClientEvent::kBulletShoot: {
      Vector2<float> position{buffer_->readFloat(message, 5),
                              buffer_->readFloat(message, 5 + sizeof(float))};
      Vector2<double> velocity{
          buffer_->readDouble(message, 5 + sizeof(float) * 2),
          buffer_->readDouble(message, 5 + sizeof(float) * 2 + sizeof(double))};
      pushEvent({type, new client_event_bullet_shoot_t{position, velocity}});
      break;
    }
  }
}

void Client::send(OutgoingClientEvent event, const void* data) noexcept {
  switch (event) {
    case OutgoingClientEvent::kUpdatePosition: {
      constexpr static int32_t size = 5 + sizeof(float) * 2;
      const auto& vector = *reinterpret_cast<const Vector2<float>*>(data);
      uint8_t message[size];
      buffer_->writeUint8(message, static_cast<uint8_t>(event), 4);
      buffer_->writeFloat(message, vector.x(), 5);
      buffer_->writeFloat(message, vector.y(), 5 + sizeof(float));
      return send(message, size);
    }
    case OutgoingClientEvent::kBulletShoot: {
      constexpr static int32_t size = 5 + sizeof(float);
      const auto& angle = *reinterpret_cast<const float*>(data);
      uint8_t message[size];
      buffer_->writeUint8(message, static_cast<uint8_t>(event), 4);
      buffer_->writeFloat(message, angle, 5);
      return send(message, size);
    }
  }
}
