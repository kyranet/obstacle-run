// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/ImageRenderer.h"
#include "interfaces/ComponentFactory.h"

class ImageRendererFactory final : public ComponentFactory<ImageRenderer*> {
 public:
  ImageRendererFactory() noexcept;
  ~ImageRendererFactory() noexcept override;
  ImageRenderer* fromJson(const Json::Value&) override;
  Json::Value toJson(ImageRenderer* value) const override;
};
