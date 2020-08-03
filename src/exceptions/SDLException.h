#pragma once
#include <stdexcept>

/**
 * \brief The SDL Exception class for any SDL-related error.
 */
class SDLException : public std::logic_error {
 public:
  /**
   * \brief Create a SDL Exception instance.
   * \param message The error message description.
   */
  explicit SDLException(const std::string &message)
      : std::logic_error("SDL Error " + message) {}
};