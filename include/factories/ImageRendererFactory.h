// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/ImageRenderer.h"
#include "interfaces/ComponentFactory.h"

class ImageRendererFactory final : public ComponentFactory<ImageRenderer*> {
  static std::string getNameFromImageFit(ImageFit value) noexcept;
  static ImageFit getImageFitFromName(const std::string& value) noexcept;

 public:
  ImageRendererFactory() noexcept;
  ~ImageRendererFactory() noexcept override;
  ImageRenderer* fromJson(const Json::Value&) override;
  Json::Value toJson(ImageRenderer* value) const override;
};