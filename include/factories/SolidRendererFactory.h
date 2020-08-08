// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <json/json.h>

#include "components/SolidRenderer.h"
#include "interfaces/ComponentFactory.h"

class SolidRendererFactory final : public ComponentFactory<SolidRenderer*> {
 public:
  SolidRendererFactory() noexcept;
  ~SolidRendererFactory() noexcept override;
  SolidRenderer* fromJson(const Json::Value&) override;
  Json::Value toJson(SolidRenderer* value) const override;
};
