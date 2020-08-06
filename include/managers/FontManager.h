// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <SDL_ttf.h>

#include <map>
#include <string>

class Font;

class FontManager {
  static FontManager* instance_;
  std::map<std::string, std::map<unsigned int, Font*>> cache_{};

  FontManager();

 public:
  ~FontManager();
  void init();

  [[nodiscard]] inline const Font* get(const std::string& name,
                                       unsigned int size) noexcept {
    return cache_[name][size];
  }

  [[nodiscard]] inline static FontManager* instance() noexcept {
    if (instance_ == nullptr) instance_ = new FontManager();
    return instance_;
  }
};
