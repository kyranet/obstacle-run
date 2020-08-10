// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "managers/FontManager.h"

#include <json/json.h>

#include <cassert>
#include <fstream>

#include "exceptions/FileSystemException.h"
#include "objects/Font.h"
#include "utils/DebugAssert.h"

std::unique_ptr<FontManager> FontManager::instance_{};

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

  debug_print("%s", "Loading Fonts...\n");
  for (const auto& object : root) {
    const auto& name = object["name"].asCString();
    const auto& path = object["path"].asCString();
    const auto& size = static_cast<uint16_t>(object["size"].asUInt());
    debug_print("Loading Font: '%s' named '%s' with size '%zi'.\n", path, name,
                size);
    assert(((void)"Detected duplicated name and size!",
            !instance_->cache_[name][size]));

    auto font = std::make_shared<Font>(path, size);
    debug_print("Successfully loaded Font '%s' with size '%zi'.\n", name, size);
    instance_->cache_[name][size] = font;
  }

  debug_print("Successfully loaded %zi Font(s).\n", instance_->cache_.size());
}
