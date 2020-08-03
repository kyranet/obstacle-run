// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <stdexcept>

/**
 * \brief The SDL Exception class for any SDL-related error.
 */
class PointerException : public std::logic_error {
 public:
  /**
   * \brief Create a Pointer Exception instance.
   * \param message The error message description.
   */
  explicit PointerException(const std::string& message)
      : std::logic_error("SDL Error " + message) {}
};