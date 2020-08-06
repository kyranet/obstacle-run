// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "managers/ImageManager.h"

#include <json/json.h>

#include <cassert>
#include <fstream>

#include "exceptions/FileSystemException.h"
#include "objects/Image.h"
#include "utils/DebugAssert.h"

ImageManager* ImageManager::instance_ = nullptr;

ImageManager::ImageManager() = default;
ImageManager::~ImageManager() = default;

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

  debug_print("%s", "Loading Images...\n");
  for (const auto& object : root) {
    const auto& name = object["name"].asCString();
    const auto& path = object["path"].asCString();
    debug_print("Loading Image: '%s' named '%s'.\n", path, name);

    auto* image = new Image(path);
    assert(((void)"Detected duplicated name and size!", !cache_[name]));
    cache_[name] = image;
    debug_print("Successfully loaded Image '%s' with size (%zi, %zi).\n", name,
                image->size().x(), image->size().y());
  }

  debug_print("Successfully loaded %zi Image(s).\n", cache_.size());
}
