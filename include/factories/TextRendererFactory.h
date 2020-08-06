// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "components/TextRenderer.h"
#include "interfaces/ComponentFactory.h"

class TextRendererFactory final : public ComponentFactory<TextRenderer*> {
 public:
  TextRendererFactory() noexcept;
  ~TextRendererFactory() noexcept override;
  TextRenderer* fromJson(const Json::Value&) override;
  Json::Value toJson(TextRenderer* value) const override;
};
