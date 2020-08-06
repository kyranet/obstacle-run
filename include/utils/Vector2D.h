// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <json/json.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

/**
 * \brief The Vector2D class that manages all vectors in the game.
 * \tparam T The value type this instance will hold and use.
 */
template <typename T,
          typename =
              typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Vector2D final {
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
   * \brief Create a Vector2D with both coordinates set as 0.
   */
  Vector2D() noexcept : x_(0), y_(0) {}
  /**
   * \brief Create a Vector2D instance with an x and y coordinates.
   * \param x The value for the x coordinate.
   * \param y The value for the y coordinate.
   */
  Vector2D(T x, T y) noexcept : x_(x), y_(y) {}

  // Copy constructors
  Vector2D(Vector2D<T>& other) noexcept : x_(other.x()), y_(other.y()) {}
  Vector2D(const Vector2D<T>& other) noexcept : x_(other.x()), y_(other.y()) {}
  Vector2D(volatile Vector2D<T>& other) noexcept
      : x_(other.x()), y_(other.y()) {}
  Vector2D(volatile const Vector2D<T>& other) noexcept
      : x_(other.x()), y_(other.y()) {}

  // Move constructor
  Vector2D(Vector2D<T>&& other) noexcept : x_(other.x()), y_(other.y()) {}

  /**
   * \brief The Vector2D destructor.
   */
  ~Vector2D() = default;

  Vector2D& operator=(const Vector2D<T>& other) {
    x() = other.x();
    y() = other.y();
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
   * \brief Set the (x, y) coordinates from another Vector2D instance.
   * \param v A vector with the (x, y) coordinates to set to this instance.
   */
  inline void set(const Vector2D<T>& v) noexcept {
    x_ = v.x();
    y_ = v.y();
  }

  template <typename Q, typename = typename std::enable_if<
                            std::is_arithmetic<Q>::value, Q>::type>
  [[nodiscard]] inline Vector2D<Q> to() const noexcept {
    return {static_cast<Q>(x()), static_cast<Q>(y())};
  }

  /**
   * \brief Gets the magnitude of this vector.
   * \return The magnitude of this vector.
   */
  [[nodiscard]] inline T magnitude() const noexcept {
    return sqrt(pow(x(), 2) + pow(y(), 2));
  }

  [[nodiscard]] inline Vector2D<T> operator-(
      const Vector2D<T>& v) const noexcept {
    return Vector2D(x() - v.x(), y() - v.y());
  }

  [[nodiscard]] inline Vector2D<T> operator+(
      const Vector2D<T>& v) const noexcept {
    return Vector2D(x() + v.x(), y() + v.y());
  }

  [[nodiscard]] inline Vector2D<T> operator*(
      const Vector2D<T>& d) const noexcept {
    return Vector2D(x() * d.x(), y() * d.y());
  }

  [[nodiscard]] inline Vector2D<T> operator*(T d) const noexcept {
    return Vector2D(x() * d, y() * d);
  }

  [[nodiscard]] inline Vector2D<T> operator/(T d) const noexcept {
    return Vector2D(x() / d, y() / d);
  }

  [[nodiscard]] static Vector2D<T> fromJson(const Json::Value& json) noexcept {
    const auto x = json[0U].asInt();
    const auto y = json[1U].asInt();
    return Vector2D(x, y);
  }

  [[nodiscard]] Json::Value toJson() const noexcept {
    Json::Value out(Json::arrayValue);
    out.append(Json::Value(x()));
    out.append(Json::Value(y()));
    return out;
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Vector2D<T>& v) noexcept {
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
  }
};