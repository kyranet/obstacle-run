#pragma once
#include "SDL.h"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

class Game final {
  static Game* instance_;
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  bool stop_ = false;

  bool init() noexcept;
  Game() noexcept;

 public:
  ~Game() noexcept;
  bool start() noexcept;
  bool end() noexcept;
  void run();

  static Game* getInstance();
};
