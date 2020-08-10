// Copyright (c) 2020 Antonio Román. All rights reserved.

#include <cstdlib>
#include <iostream>

#include "Game.h"
#include "managers/ComponentManager.h"
#include "managers/FontManager.h"
#include "managers/ImageManager.h"

#undef main

int main(int, char*[]) {
#if _DEBUG  // Non-standard _DEBUG is used because crtdbg.h is a MSVC-specific
            // header, it is also not included because MSVC imports it in all
            // programs.
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
                 _CRTDBG_LEAK_CHECK_DF);  // Check Memory Leaks
#endif
  ComponentManager::create();
  ImageManager::create();
  FontManager::create();

  auto* game = Game::getInstance();
  game->start();
  try {
    game->run();
    game->end();
    return EXIT_SUCCESS;
  } catch (const std::exception& exception) {
    std::cerr << exception.what();
    game->end();
    return EXIT_FAILURE;
  }
}
