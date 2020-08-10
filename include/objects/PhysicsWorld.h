// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <map>

#include "utils/Vector4.h"

class PhysicsWorld {
  uint32_t index_{0};
  std::map<uint32_t, Vector4<int32_t>> objects_;

 public:
  inline uint32_t add(
      std::remove_reference_t<Vector4<int32_t>>& object) noexcept {
    const auto key = ++index_;
    objects_.insert({key, std::move(object)});
    return key;
  }

  inline void remove(uint32_t id) noexcept { objects_.erase(id); }

  [[nodiscard]] inline const Vector4<int32_t>& at(uint32_t id) const noexcept {
    return objects_.at(id);
  }

  [[nodiscard]] uint32_t collides(const Vector2<int32_t>& l1,
                                  const Vector2<int32_t>& size) const noexcept {
    const Vector4<int32_t> r1{l1.x(), l1.y(), l1.x() + size.x(),
                              l1.y() + size.y()};
    for (const auto& object : objects_) {
      if (r1.overlaps(object.second)) return object.first;
    }

    return max();
  }

  [[nodiscard]] inline uint32_t collides(
      const Vector2<float>& point,
      const Vector2<int32_t>& size) const noexcept {
    return collides(Vector2<int32_t>(point), size);
  }

  [[nodiscard]] inline static uint32_t max() noexcept {
    return std::numeric_limits<uint32_t>::max();
  }
};
