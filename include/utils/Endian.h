// Copyright (c) 2020 Antonio Román. All rights reserved.
// From:
// https://codereview.stackexchange.com/questions/45675/checking-endianness-at-compile-time

#pragma once
#include <cstdint>

enum class Endian : uint32_t {
  kLittleEndian = 0x00000001,
  kBigEndian = 0x01000000,
  kPdpEndian = 0x00010000,
  kUnknownEndian = 0xFFFFFFFF
};

/**
 * A simple function that can be used to help determine a program's endianness
 * at compile-time.
 */
constexpr Endian endian() {
  return (static_cast<uint32_t>(1 & 0xFFFFFFFF) ==
          static_cast<uint32_t>(Endian::kLittleEndian))
             ? Endian::kLittleEndian
             : (static_cast<uint32_t>(1 & 0xFFFFFFFF) ==
                static_cast<uint32_t>(Endian::kBigEndian))
                   ? Endian::kBigEndian
                   : (static_cast<uint32_t>(1 & 0xFFFFFFFF) ==
                      static_cast<uint32_t>(Endian::kPdpEndian))
                         ? Endian::kPdpEndian
                         : Endian::kUnknownEndian;
}

/**
 * A simple function that can be used to help determine if a program's
 * endianness is Little Endian at compile-time.
 */
constexpr bool littleEndian() { return endian() == Endian::kLittleEndian; }
