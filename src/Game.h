#include "SDL.h"

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 480;

class Game final {
  static Game* instance_;
  SDL_Window* window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;
  SDL_Surface* surface_ = nullptr;
  bool init();
  Game();

 public:
  ~Game();
  bool start();
  bool end();

  static Game* getInstance();
};
