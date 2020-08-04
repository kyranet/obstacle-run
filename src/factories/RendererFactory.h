// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "../components/Renderer.h"
#include "../interfaces/ComponentFactory.h"

class RendererFactory final : public ComponentFactory<Renderer*> {
 public:
  RendererFactory() noexcept;
  ~RendererFactory() noexcept override;
  Renderer* fromJson(const Json::Value&) override;
  Json::Value toJson(Renderer* value) override;
};
