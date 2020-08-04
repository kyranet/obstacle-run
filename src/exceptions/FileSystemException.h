// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <stdexcept>

/**
 * \brief The File System Exception class for any FS-related error.
 */
class FileSystemException : public std::logic_error {
 public:
  /**
   * \brief Create a FS Exception instance.
   * \param message The error message description.
   */
  explicit FileSystemException(const std::string& message)
      : std::logic_error("FS Exception: " + message) {}
};