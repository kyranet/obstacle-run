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
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
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

  explicit Vector2(const Json::Value& json) noexcept {
    if constexpr (std::is_same_v<T, float>) {
      x_ = json[0U].asFloat();
      y_ = json[1U].asFloat();
    } else if constexpr (std::is_same_v<T, double>) {
      x_ = json[0U].asDouble();
      y_ = json[1U].asDouble();
    } else if constexpr (std::is_same_v<T, int>) {
      x_ = json[0U].asInt();
      y_ = json[1U].asInt();
    } else if constexpr (std::is_same_v<T, unsigned int>) {
      x_ = json[0U].asUInt();
      y_ = json[1U].asUInt();
    } else if constexpr (std::is_same_v<T, int64_t>) {
      x_ = json[0U].asInt64();
      y_ = json[1U].asInt64();
    } else if constexpr (std::is_same_v<T, uint64_t>) {
      x_ = json[0U].asUInt64();
      y_ = json[1U].asUInt64();
    } else if constexpr (std::is_same_v<T, uint32_t>) {
      x_ = json[0U].asUInt();
      y_ = json[1U].asUInt();
    } else {
      x_ = static_cast<T>(json[0U].asDouble());
      y_ = static_cast<T>(json[1U].asDouble());
    }
  }

  explicit Vector2(const SDL_Point& point) noexcept {
    x_ = static_cast<T>(point.x);
    y_ = static_cast<T>(point.y);
  }
  template <typename Q, typename = typename std::enable_if_t<
                            std::is_convertible_v<T, Q>, Q>>
  explicit Vector2(const Vector2<Q>& other) noexcept
      : x_(static_cast<T>(other.x())), y_(static_cast<T>(other.y())) {}

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

  /**
   * \brief Gets the magnitude of this vector.
   * \return The magnitude of this vector.
   */
  [[nodiscard]] inline T magnitude() const noexcept {
    return sqrt(pow(x(), 2) + pow(y(), 2));
  }

  template <typename Q, typename = typename std::enable_if_t<
                            std::is_convertible_v<T, Q>, Q>>
  [[nodiscard]] inline Vector2<T> operator-(
      const Vector2<Q>& v) const noexcept {
    return Vector2(x() - static_cast<T>(v.x()), y() - static_cast<T>(v.y()));
  }

  template <typename Q, typename = typename std::enable_if_t<
                            std::is_convertible_v<T, Q>, Q>>
  [[nodiscard]] inline Vector2<T> operator+(
      const Vector2<Q>& v) const noexcept {
    return Vector2(x() + static_cast<T>(v.x()), y() + static_cast<T>(v.y()));
  }

  template <typename Q, typename = typename std::enable_if_t<
                            std::is_convertible_v<T, Q>, Q>>
  [[nodiscard]] inline Vector2<T> operator*(
      const Vector2<Q>& d) const noexcept {
    return Vector2(x() * static_cast<T>(d.x()), y() * static_cast<T>(d.y()));
  }

  template <typename Q, typename = typename std::enable_if_t<
                            std::is_convertible_v<T, Q>, Q>>
  [[nodiscard]] inline Vector2<T> operator*(Q d) const noexcept {
    return Vector2(x() * static_cast<T>(d), y() * static_cast<T>(d));
  }

  template <typename Q, typename = typename std::enable_if_t<
                            std::is_convertible_v<T, Q>, Q>>
  [[nodiscard]] inline Vector2<T> operator/(Q d) const noexcept {
    return Vector2(x() / static_cast<T>(d), y() / static_cast<T>(d));
  }

  [[nodiscard]] Json::Value toJson() const noexcept {
    Json::Value out(Json::arrayValue);
    out.append(Json::Value(x()));
    out.append(Json::Value(y()));
    return out;
  }

  [[nodiscard]] SDL_Point toPoint() const noexcept {
    return {static_cast<int>(x()), static_cast<int>(y())};
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const Vector2<T>& v) noexcept {
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
  }
};