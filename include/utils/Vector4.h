// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <SDL.h>
#include <json/json.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>

#include "Vector2.h"

/**
 * \brief The Vector2 class that manages all vectors in the game.
 * \tparam T The value type this instance will hold and use.
 */
template <typename T,
          typename =
              typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Vector4 final {
  /**
   * \brief The x coordinate.
   */
  T x_;
  /**
   * \brief The y coordinate.
   */
  T y_;

  /**
   * \brief The z coordinate.
   */
  T z_;

  /**
   * \brief The height.
   */
  T a_;

 public:
  // Default constructors

  /**
   * \brief Create a Vector2 with both coordinates set as 0.
   */
  Vector4() noexcept : x_(0), y_(0), z_(0), a_(0) {}
  /**
   * \brief Create a Vector2 instance with an x and y coordinates.
   * \param x The value for the x coordinate.
   * \param y The value for the y coordinate.
   */
  Vector4(T x, T y, T z, T a) noexcept : x_(x), y_(y), z_(z), a_(a) {}

  explicit Vector4(const Json::Value& json) noexcept
      : x_(static_cast<T>(json[0U].asInt())),
        y_(static_cast<T>(json[1U].asInt())),
        z_(static_cast<T>(json[2U].asInt())),
        a_(static_cast<T>(json[3U].asInt())) {}
  explicit Vector4(const SDL_Rect& point) noexcept
      : x_(static_cast<T>(point.x)),
        y_(static_cast<T>(point.y)),
        z_(static_cast<T>(point.w)),
        a_(static_cast<T>(point.h)) {}

  // Copy constructors
  Vector4(Vector4<T>& other) noexcept
      : x_(other.x()), y_(other.y()), z_(other.z()), a_(other.a()) {}
  Vector4(const Vector4<T>& other) noexcept
      : x_(other.x()), y_(other.y()), z_(other.z()), a_(other.a()) {}
  Vector4(volatile Vector4<T>& other) noexcept
      : x_(other.x()), y_(other.y()), z_(other.z()), a_(other.a()) {}
  Vector4(volatile const Vector4<T>& other) noexcept
      : x_(other.x()), y_(other.y()), z_(other.z()), a_(other.a()) {}

  // Move constructor
  Vector4(Vector4<T>&& other) noexcept
      : x_(other.x()), y_(other.y()), z_(other.z()), a_(other.a()) {}

  /**
   * \brief The Vector2 destructor.
   */
  ~Vector4() = default;

  Vector4& operator=(const Vector4<T>& other) {
    x() = other.x();
    y() = other.y();
    z() = other.z();
    a() = other.h();
    return *this;
  }

  Vector4& operator=(const SDL_Rect& other) {
    x() = other.x;
    y() = other.y;
    z() = other.w;
    a() = other.h;
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
   * \brief Get the z coordinate value for this instance.
   * \return The value for the w coordinate.
   */
  [[nodiscard]] inline const T& z() const noexcept { return z_; }

  /**
   * \brief Get the height value for this instance.
   * \return The value for the height.
   */
  [[nodiscard]] inline const T& a() const noexcept { return a_; }

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
   * \brief Set the z coordinate value for this instance.
   * \param x The new value for the w coordinate.
   */
  inline T& z() noexcept { return z_; }

  /**
   * \brief Set the height value for this instance.
   * \param y The new value for the height.
   */
  inline T& a() noexcept { return a_; }

  /**
   * \brief Set the (x, y) coordinates.
   * \param x The new value for the x coordinate.
   * \param y The new value for the y coordinate.
   */
  inline void set(T x, T y, T z, T a) noexcept {
    x_ = x;
    y_ = y;
    z_ = z;
    a_ = a;
  }

  /**
   * \brief Set the (x, y) coordinates from another Vector2 instance.
   * \param v A vector with the (x, y) coordinates to set to this instance.
   */
  inline void set(const Vector4<T>& v) noexcept {
    x_ = v.x();
    y_ = v.y();
    z_ = v.z();
    a_ = v.a();
  }

  template <typename Q, typename = typename std::enable_if<
                            std::is_arithmetic<Q>::value, Q>::type>
  [[nodiscard]] inline Vector4<Q> toJson() const noexcept {
    return {static_cast<Q>(x()), static_cast<Q>(y()), static_cast<Q>(z()),
            static_cast<Q>(a())};
  }

  [[nodiscard]] inline Vector4<T> operator-(
      const Vector4<T>& v) const noexcept {
    return Vector4(x() - v.x(), y() - v.y(), z() - v.z(), a() - v.a());
  }

  [[nodiscard]] inline Vector4<T> operator-(
      const Vector2<T>& v) const noexcept {
    return Vector4(x() - v.x(), y() - v.y(), z(), a());
  }

  [[nodiscard]] inline Vector4<T> operator+(
      const Vector4<T>& v) const noexcept {
    return Vector4(x() + v.x(), y() + v.y(), z() + v.z(), a() + v.a());
  }

  [[nodiscard]] inline Vector4<T> operator+(
      const Vector2<T>& v) const noexcept {
    return Vector4(x() + v.x(), y() + v.y(), z(), a());
  }

  [[nodiscard]] inline Vector4<T> operator*(
      const Vector4<T>& d) const noexcept {
    return Vector4(x() * d.x(), y() * d.y(), z() * d.z(), a() * d.a());
  }

  [[nodiscard]] inline Vector4<T> operator*(
      const Vector2<T>& d) const noexcept {
    return Vector4(x() * d.x(), y() * d.y(), z(), a());
  }

  [[nodiscard]] inline Vector4<T> operator*(T d) const noexcept {
    return Vector4(x() * d, y() * d, z() * d, a() * d);
  }

  [[nodiscard]] inline Vector4<T> operator/(T d) const noexcept {
    return Vector4(x() / d, y() / d, z() / d, a() / d);
  }

  [[nodiscard]] Json::Value toJson() const noexcept {
    Json::Value out(Json::arrayValue);
    out.append(Json::Value(x()));
    out.append(Json::Value(y()));
    out.append(Json::Value(z()));
    out.append(Json::Value(a()));
    return out;
  }

  [[nodiscard]] SDL_Rect toRectangle() const noexcept {
    const auto x = static_cast<int>(x_);
    const auto y = static_cast<int>(y_);
    const auto z = static_cast<int>(z_);
    const auto a = static_cast<int>(a_);
    return {x, y, z, a};
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Vector4<T>& v) noexcept {
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.h()
       << ")";
    return os;
  }
};