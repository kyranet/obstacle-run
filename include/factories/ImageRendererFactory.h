// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/ImageRenderer.h"
#include "interfaces/ComponentFactory.h"

class ImageRendererFactory final : public ComponentFactory<ImageRenderer> {
  static ImageFit getImageFitFromName(const std::string& value) noexcept;

 public:
  ImageRendererFactory() noexcept;
  ~ImageRendererFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<ImageRenderer> fromJson(
      const Json::Value& json,
      std::weak_ptr<GameObject> parent) const noexcept override;
};
