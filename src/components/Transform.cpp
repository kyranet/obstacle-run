// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/Transform.h"

#include <utility>

Transform::Transform(Vector2<int32_t> position, Vector2<int32_t> scale) noexcept
    : position_(std::move(position)), scale_(std::move(scale)) {}

Transform::~Transform() noexcept = default;
