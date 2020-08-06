// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <map>
#include <string>

class Image;

class ImageManager {
  static ImageManager* instance_;
  std::map<std::string, Image*> cache_{};

  ImageManager();

 public:
  ~ImageManager();
  void init();

  [[nodiscard]] inline Image* get(const std::string& name) noexcept {
    return cache_[name];
  }

  [[nodiscard]] inline static ImageManager* instance() noexcept {
    if (instance_ == nullptr) instance_ = new ImageManager();
    return instance_;
  }
};
