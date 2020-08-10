// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <string>

class Image;

class ImageManager {
  static std::unique_ptr<ImageManager> instance_;
  std::map<std::string, std::shared_ptr<Image>> cache_;

 public:
  static void init();

  [[nodiscard]] inline static const std::shared_ptr<Image>& get(
      const std::string& name) noexcept {
    return instance_->cache_[name];
  }

  static inline void create() {
    assert(
        ((void)"FontManager::create() must be called only once", !instance_));
    instance_ = std::make_unique<ImageManager>();
  }

  static inline void close() { instance_->cache_.clear(); }
};
