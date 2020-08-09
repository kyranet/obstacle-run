// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <SDL.h>
#include <json/json.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

/**
 * \brief The Vector2 class that manages all vectors in the game.
 * \tparam T The value type this instance will hold and use.
 */
template <typename T,
          typename =
              typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Vector2 final {
  /**
   * \brief The x coordinate.
   */
  T x_;
  /**
   * \brief The y coordinate.
   */
  T y_;

 public:
  // Default constructors

  /**
   * \brief Create a Vector2 with both coordinates set as 0.
   */
  Vector2() noexcept : x_(0), y_(0) {}
  /**
   * \brief Create a Vector2 instance with an x and y coordinates.
   * \param x The value for the x coordinate.
   * \param y The value for the y coordinate.
   */
  Vector2(T x, T y) noexcept : x_(x), y_(y) {}

  // Copy constructors
  Vector2(Vector2<T>& other) noexcept : x_(other.x()), y_(other.y()) {}
  Vector2(const Vector2<T>& other) noexcept : x_(other.x()), y_(other.y()) {}
  Vector2(volatile Vector2<T>& other) noexcept : x_(other.x()), y_(other.y()) {}
  Vector2(volatile const Vector2<T>& other) noexcept
      : x_(other.x()), y_(other.y()) {}

  // Move constructor
  Vector2(Vector2<T>&& other) noexcept : x_(other.x()), y_(other.y()) {}

  /**
   * \brief The Vector2 destructor.
   */
  ~Vector2() = default;

  Vector2& operator=(const Vector2<T>& other) {
    x() = other.x();
    y() = other.y();
    return *this;
  }

  Vector2& operator=(const SDL_Point& other) {
    x() = other.x;
    y() = other.y;
    return *this;
  }

  /**
   * \brief Get the x coordinate value for this instance.
   * \return The value for the x coordinate.
   */
  [[nodiscard]] inline const T& x() const noexcept { return x_; }

  /**
   * \brief Get the y coordinate value for this instance.
   * \return The value for the y coordinate.
   */
  [[nodiscard]] inline const T& y() const noexcept { return y_; }

  /**
   * \brief Set the x coordinate value for this instance.
   * \param x The new value for the x coordinate.
   */
  inline T& x() noexcept { return x_; }

  /**
   * \brief Set the y coordinate value for this instance.
   * \param y The new value for the y coordinate.
   */
  inline T& y() noexcept { return y_; }

  /**
   * \brief Set the (x, y) coordinates.
   * \param x The new value for the x coordinate.
   * \param y The new value for the y coordinate.
   */
  inline void set(T x, T y) noexcept {
    x_ = x;
    y_ = y;
  }

  /**
   * \brief Set the (x, y) coordinates from another Vector2 instance.
   * \param v A vector with the (x, y) coordinates to set to this instance.
   */
  inline void set(const Vector2<T>& v) noexcept {
    x_ = v.x();
    y_ = v.y();
  }

  template <typename Q, typename = typename std::enable_if<
                            std::is_arithmetic<Q>::value, Q>::type>
  [[nodiscard]] inline Vector2<Q> toJson() const noexcept {
    return {static_cast<Q>(x()), static_cast<Q>(y())};
  }

  /**
   * \brief Gets the magnitude of this vector.
   * \return The magnitude of this vector.
   */
  [[nodiscard]] inline T magnitude() const noexcept {
    return sqrt(pow(x(), 2) + pow(y(), 2));
  }

  [[nodiscard]] inline Vector2<T> operator-(
      const Vector2<T>& v) const noexcept {
    return Vector2(x() - v.x(), y() - v.y());
  }

  [[nodiscard]] inline Vector2<T> operator+(
      const Vector2<T>& v) const noexcept {
    return Vector2(x() + v.x(), y() + v.y());
  }

  [[nodiscard]] inline Vector2<T> operator*(
      const Vector2<T>& d) const noexcept {
    return Vector2(x() * d.x(), y() * d.y());
  }

  [[nodiscard]] inline Vector2<T> operator*(T d) const noexcept {
    return Vector2(x() * d, y() * d);
  }

  [[nodiscard]] inline Vector2<T> operator/(T d) const noexcept {
    return Vector2(x() / d, y() / d);
  }

  [[nodiscard]] inline static Vector2<T> from(
      const Json::Value& json) noexcept {
    if constexpr (std::is_same_v<T, float_t>) {
      const auto x = json[0U].asFloat();
      const auto y = json[1U].asFloat();
      return Vector2(x, y);
    } else if constexpr (std::is_same_v<T, double_t>) {
      const auto x = json[0U].asDouble();
      const auto y = json[1U].asDouble();
      return Vector2(x, y);
    } else if constexpr (std::is_same_v<T, int>) {
      const auto x = json[0U].asInt();
      const auto y = json[1U].asInt();
      return Vector2(x, y);
    } else if constexpr (std::is_same_v<T, unsigned int>) {
      const auto x = json[0U].asUInt();
      const auto y = json[1U].asUInt();
      return Vector2(x, y);
    } else if constexpr (std::is_same_v<T, int64_t>) {
      const auto x = json[0U].asInt64();
      const auto y = json[1U].asInt64();
      return Vector2(x, y);
    } else if constexpr (std::is_same_v<T, uint64_t>) {
      const auto x = json[0U].asUInt64();
      const auto y = json[1U].asUInt64();
      return Vector2(x, y);
    } else if constexpr (std::is_same_v<T, uint32_t>) {
      const auto x = json[0U].asUInt();
      const auto y = json[1U].asUInt();
      return Vector2(x, y);
    } else {
      const auto x = json[0U].asInt();
      const auto y = json[1U].asInt();
      return Vector2(x, y);
    }
  }

  [[nodiscard]] Json::Value toJson() const noexcept {
    Json::Value out(Json::arrayValue);
    out.append(Json::Value(x()));
    out.append(Json::Value(y()));
    return out;
  }

  [[nodiscard]] static Vector2<T> from(const SDL_Point& point) noexcept {
    const auto x = static_cast<T>(point.x);
    const auto y = static_cast<T>(point.y);
    return {x, y};
  }

  [[nodiscard]] SDL_Point toPoint() const noexcept {
    const auto x = static_cast<int>(x());
    const auto y = static_cast<int>(y());
    return {x, y};
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Vector2<T>& v) noexcept {
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
  }
};