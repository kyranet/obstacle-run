// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <mutex>
#include <shared_mutex>
#include <vector>

class Server {
  enum class ClientStatus { kPending, kRunning, kClosed };

  struct client_event_t {};

  class ServerClient {
    ClientStatus status_ = ClientStatus::kPending;
    std::vector<client_event_t> events_;
    std::mutex event_mutex_;

   public:
    [[nodiscard]] inline bool isPending() const noexcept {
      return status_ == ClientStatus::kPending;
    }

    [[nodiscard]] inline bool isRunning() const noexcept {
      return status_ == ClientStatus::kRunning;
    }

    [[nodiscard]] inline bool isClosed() const noexcept {
      return status_ == ClientStatus::kClosed;
    }

    inline void pushEvent(const client_event_t& event) noexcept {
      std::lock_guard<std::mutex> guard(event_mutex_);
      events_.emplace_back(event);
    }
  };
};
