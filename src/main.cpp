// Copyright (c) 2020 Antonio Román. All rights reserved.

#include <cstdlib>
#include <iostream>

#include "Game.h"
#include "managers/ComponentManager.h"
#include "managers/FontManager.h"
#include "managers/ImageManager.h"
#include "server/Server.h"

#undef main

int main(int argc, char** argv) {
#if _DEBUG  // Non-standard _DEBUG is used because crtdbg.h is a MSVC-specific
            // header, it is also not included because MSVC imports it in all
            // programs.
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
                 _CRTDBG_LEAK_CHECK_DF);  // Check Memory Leaks
#endif
  try {
    if (argc == 2 && strcmp(argv[1], "server") == 0) {
      auto server = std::make_unique<Server>();
      server->run();
    } else {
      ComponentManager::create();
      ImageManager::create();
      FontManager::create();

      auto* game = Game::getInstance();
      game->start();
      game->run();
      game->end();
    }
    return EXIT_SUCCESS;
  } catch (const std::exception& exception) {
    std::cerr << exception.what();
    return EXIT_FAILURE;
  }
}
