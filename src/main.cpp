// Copyright (c) 2020 Antonio Román. All rights reserved.

#include <iostream>

#include "Game.h"

#undef main

int main(int, char*[]) {
#if _DEBUG  // Non-standard _DEBUG is used because crtdbg.h is a MSVC-specific
            // header, it is also not included because MSVC imports it in all
            // programs.
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
                 _CRTDBG_LEAK_CHECK_DF);  // Check Memory Leaks
#endif
  auto* game = Game::getInstance();
  game->start();
  try {
    game->run();
    return 0;
  } catch (const std::exception& exception) {
    std::cerr << exception.what();
    game->end();
    return 1;
  }
}
