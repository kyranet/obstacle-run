// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "components/TextRenderer.h"
#include "interfaces/ComponentFactory.h"

class TextRendererFactory final : public ComponentFactory<TextRenderer> {
 public:
  TextRendererFactory() noexcept;
  ~TextRendererFactory() noexcept override;

  [[nodiscard]] std::shared_ptr<TextRenderer> fromJson(
      const Json::Value& json,
      std::weak_ptr<GameObject> parent) const noexcept override;
};
