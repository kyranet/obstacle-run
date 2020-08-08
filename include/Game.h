// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

struct SDL_Window;
struct SDL_Renderer;

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

class Game final {
  static Game* instance_;
  static SDL_Window* window_;
  static SDL_Renderer* renderer_;
  bool stop_ = false;

  bool init() noexcept;
  Game() noexcept;

 public:
  ~Game() noexcept;
  bool start() noexcept;
  bool end() noexcept;
  void run();

  [[nodiscard]] inline static SDL_Window* window() noexcept { return window_; }

  [[nodiscard]] inline static SDL_Renderer* renderer() noexcept {
    return renderer_;
  }

  static Game* getInstance();
};
