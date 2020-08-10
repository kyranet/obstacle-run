// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <cassert>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

class Font;

class FontManager {
  static std::unique_ptr<FontManager> instance_;
  std::map<std::string, std::map<uint16_t, std::shared_ptr<Font>>> cache_;

 public:
  static void init();

  [[nodiscard]] inline static const std::shared_ptr<Font>& get(
      const std::string& name, uint16_t size) noexcept {
    return instance_->cache_[name][size];
  }

  static inline void create() {
    assert(
        ((void)"FontManager::create() must be called only once", !instance_));
    instance_ = std::make_unique<FontManager>();
  }

  static inline void close() { instance_->cache_.clear(); }
};
