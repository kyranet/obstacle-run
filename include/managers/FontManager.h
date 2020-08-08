// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <cstdint>
#include <map>
#include <string>

class Font;

class FontManager {
  static FontManager* instance_;
  std::map<std::string, std::map<uint16_t, Font*>> cache_{};

  FontManager();

 public:
  ~FontManager();
  void init();

  [[nodiscard]] inline const Font* get(const std::string& name,
                                       uint16_t size) noexcept {
    return cache_[name][size];
  }

  [[nodiscard]] inline static FontManager* instance() noexcept {
    if (instance_ == nullptr) instance_ = new FontManager();
    return instance_;
  }
};
