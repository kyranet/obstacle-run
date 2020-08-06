// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <SDL.h>

#include "objects/Component.h"

class Transform;

class ImageRenderer final : public Component {
  std::string path_;
  Transform* transform_ = nullptr;
  SDL_Texture* texture_ = nullptr;

 public:
  explicit ImageRenderer(std::string path) noexcept;
  ~ImageRenderer() noexcept;

  void onAwake() noexcept override;
  void onUpdate() noexcept override;

  [[nodiscard]] inline const std::string& path() const noexcept {
    return path_;
  }
  inline std::string& path() noexcept { return path_; }
};
