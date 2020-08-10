// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "managers/ImageManager.h"

#include <json/json.h>

#include <cassert>
#include <fstream>

#include "exceptions/FileSystemException.h"
#include "objects/Image.h"
#include "utils/DebugAssert.h"

std::unique_ptr<ImageManager> ImageManager::instance_ = nullptr;

/** {
  "name": "ForestBackground",
  "path": "assets/images/ForestBackground.jpg"
} */
void ImageManager::init() {
  std::string file = "./assets/images/all.json";
  Json::Value root;
  Json::CharReaderBuilder builder;
  std::ifstream stream(file, std::ifstream::binary);
  std::string errors;
  if (!Json::parseFromStream(builder, stream, &root, &errors)) {
    throw FileSystemException("Could not parse JSON body from '" + file +
                              "'. Reason: " + errors);
  }

  // Hold a reference of the instance's cache
  auto& cache = instance_->cache_;

  debug_print("%s", "Loading Images...\n");
  for (const auto& object : root) {
    const auto& name = object["name"].asCString();
    const auto& path = object["path"].asCString();
    debug_print("Loading Image: '%s' named '%s'.\n", path, name);
    assert(((void)"Detected duplicated name and size!", !cache[name]));

    auto image = std::make_shared<Image>(path);
    debug_print("Successfully loaded Image '%s' with size (%zi, %zi).\n", name,
                image->size().x(), image->size().y());
    cache[name] = std::move(image);
  }

  debug_print("Successfully loaded %zi Image(s).\n", cache.size());
}
