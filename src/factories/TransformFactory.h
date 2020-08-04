// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "../components/Transform.h"
#include "../interfaces/ComponentFactory.h"

class TransformFactory final : public ComponentFactory<Transform*> {
 public:
  TransformFactory() noexcept;
  ~TransformFactory() noexcept override;
  Transform* fromJson(const Json::Value&) override;
  Json::Value toJson(Transform* value) override;
};
