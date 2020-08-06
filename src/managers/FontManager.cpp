// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "managers/FontManager.h"

#include <json/json.h>

#include <cassert>
#include <fstream>

#include "exceptions/FileSystemException.h"
#include "objects/Font.h"
#include "utils/DebugAssert.h"

FontManager* FontManager::instance_ = nullptr;

FontManager::FontManager() = default;
FontManager::~FontManager() = default;

void FontManager::init() {
  std::string file = "./assets/fonts/all.json";
  Json::Value root;
  Json::CharReaderBuilder builder;
  std::ifstream stream(file, std::ifstream::binary);
  std::string errors;
  if (!Json::parseFromStream(builder, stream, &root, &errors)) {
    throw FileSystemException("Could not parse JSON body from '" + file +
                              "'. Reason: " + errors);
  }

  debug_print("Loading Fonts...\n");
  for (const auto& object : root) {
    const auto& name = object["name"].asCString();
    const auto& path = object["path"].asCString();
    const auto& size = object["size"].asInt();
    debug_print("Loading Font: '%s' named '%s' with size '%zi'.\n", path, name,
                size);

    auto* font = new Font(path, size);
    assert(((void)"Detected duplicated name and size!", !cache_[name][size]));
    cache_[name][size] = font;
    debug_print("Successfully loaded Font '%s' with size '%zi'.\n", name, size);
  }

  debug_print("Successfully loaded %zi Font(s).\n", cache_.size());
}
