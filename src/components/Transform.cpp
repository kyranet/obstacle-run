// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/Transform.h"

#include <utility>

Transform::Transform(Vector2D<int> position, Vector2D<int> scale) noexcept
    : position_(std::move(position)), scale_(std::move(scale)) {}

Transform::~Transform() noexcept = default;
