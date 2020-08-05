// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/Transform.h"

Transform::Transform(Vector2D<int> position, Vector2D<int> scale) noexcept
    : position_(position), scale_(scale) {}

Transform::~Transform() noexcept = default;

Vector2D<int> Transform::getPosition() const noexcept { return position_; }
void Transform::setPosition(int x, int y) noexcept { position_.set(x, y); }
void Transform::setPosition(Vector2D<int> position) noexcept {
  position_ = position;
}

Vector2D<int> Transform::getScale() const noexcept { return scale_; }
void Transform::setScale(Vector2D<int> scale) noexcept { scale_ = scale; }
