// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>

#include "Endian.h"

class Buffer {
  float floatArray_[1];
  double doubleArray_[1];

 public:
  inline void writeInt8(uint8_t* buffer, int8_t input,
                        size_t offset) const noexcept {
    reinterpret_cast<int8_t*>(buffer + offset)[0] = input;
  }

  [[nodiscard]] inline int8_t readInt8(const uint8_t* buffer,
                                       size_t offset) const noexcept {
    return reinterpret_cast<const int8_t*>(buffer)[offset];
  }

  inline void writeUint8(uint8_t* buffer, uint8_t input,
                         size_t offset) const noexcept {
    buffer[offset] = input;
  }

  [[nodiscard]] inline uint8_t readUint8(const uint8_t* buffer,
                                         size_t offset) const noexcept {
    return buffer[offset];
  }

  inline void writeInt16(uint8_t* buffer, int16_t input,
                         size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<int16_t*>(buffer + offset)[0] = input;
    } else {
      buffer[offset] = input & 0xFF;
      buffer[offset + 1] = (input >> 8);
    }
  }

  [[nodiscard]] inline int16_t readInt16(const uint8_t* buffer,
                                         size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      return reinterpret_cast<const int16_t*>(buffer + offset)[0];
    } else {
      static const auto& valuePower = static_cast<int16_t>(pow(2, 8));
      static const auto& signPower = static_cast<int16_t>(pow(2, 15));
      const auto& first = buffer[offset];
      const auto& last = buffer[offset + 1];
      const auto val = first + last * valuePower;
      return val | (val & signPower) * 0x1fffe;
    }
  }

  inline void writeUint16(uint8_t* buffer, uint16_t input,
                          size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<uint16_t*>(buffer + offset)[0] = input;
    } else {
      buffer[offset] = input & 0xFF;
      buffer[offset + 1] = (input >> 8);
    }
  }

  [[nodiscard]] inline uint16_t readUInt16(const uint8_t* buffer,
                                           size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      return reinterpret_cast<const uint16_t*>(buffer + offset)[0];
    } else {
      static const auto& valuePower = static_cast<int16_t>(pow(2, 8));
      const auto& first = buffer[offset];
      const auto& last = buffer[offset + 1];
      return first + last * valuePower;
    }
  }

  inline void writeInt32(uint8_t* buffer, int32_t input,
                         size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<int32_t*>(buffer + offset)[0] = input;
    } else {
      buffer[offset] = input & 0xFF;
      buffer[offset + 1] = (input >> 8) & 0xFF;
      buffer[offset + 2] = (input >> 16) & 0xFF;
      buffer[offset + 3] = (input >> 24);
    }
  }

  [[nodiscard]] inline int32_t readInt32(const uint8_t* buffer,
                                         size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      return reinterpret_cast<const int32_t*>(buffer + offset)[0];
    } else {
      static const auto& powerOf8 = static_cast<int32_t>(pow(2, 8));
      static const auto& powerOf16 = static_cast<int32_t>(pow(2, 16));
      return buffer[offset] + buffer[offset + 1] * powerOf8 +
             buffer[offset + 2] * powerOf16 +
             (buffer[offset + 3] << 24);  // Overflow
    }
  }

  inline void writeUint32(uint8_t* buffer, uint32_t input,
                          size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<uint32_t*>(buffer + offset)[0] = input;
    } else {
      buffer[offset] = input & 0xFF;
      buffer[offset + 1] = (input >> 8) & 0xFF;
      buffer[offset + 2] = (input >> 16) & 0xFF;
      buffer[offset + 3] = (input >> 24);
    }
  }

  [[nodiscard]] inline uint32_t readUInt32(const uint8_t* buffer,
                                           size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      return reinterpret_cast<const uint32_t*>(buffer + offset)[0];
    } else {
      return (static_cast<uint32_t>(readUint8(buffer, offset)) << 24U) +
             (static_cast<uint32_t>(readUint8(buffer, offset + 1)) << 16U) +
             (static_cast<uint32_t>(readUint8(buffer, offset + 2)) << 8U) +
             (static_cast<uint32_t>(readUint8(buffer, offset + 3)));
    }
  }

  inline void writeFloat(uint8_t* buffer, float input, size_t offset) noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<float*>(buffer + offset)[0] = input;
    } else {
      floatArray_[0] = input;
      const auto* temporary = reinterpret_cast<uint8_t*>(floatArray_);
      buffer[offset] = temporary[3];
      buffer[offset + 1] = temporary[2];
      buffer[offset + 2] = temporary[1];
      buffer[offset + 3] = temporary[0];
    }
  }

  [[nodiscard]] inline float readFloat(const uint8_t* buffer,
                                       size_t offset) noexcept {
    if constexpr (littleEndian()) {
      return reinterpret_cast<const float*>(buffer + offset)[0];
    } else {
      auto* temporary = reinterpret_cast<uint8_t*>(floatArray_);
      temporary[3] = buffer[offset];
      temporary[2] = buffer[offset + 1];
      temporary[1] = buffer[offset + 2];
      temporary[0] = buffer[offset + 3];
      return floatArray_[0];
    }
  }

  inline void writeInt64(uint8_t* buffer, int64_t input,
                         size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<int64_t*>(buffer + offset)[0] = input;
    } else {
      buffer[offset] = input & 0xFF;
      buffer[offset + 1] = (input >> 8) & 0xFF;
      buffer[offset + 2] = (input >> 16) & 0xFF;
      buffer[offset + 3] = (input >> 24) & 0xFF;
      buffer[offset + 4] = (input >> 32) & 0xFF;
      buffer[offset + 5] = (input >> 40) & 0xFF;
      buffer[offset + 6] = (input >> 48) & 0xFF;
      buffer[offset + 7] = (input >> 56);
    }
  }

  [[nodiscard]] inline int64_t readInt64(const uint8_t* buffer,
                                         size_t offset) const noexcept {
    if (littleEndian()) {
      return reinterpret_cast<const int64_t*>(buffer + offset)[0];
    } else {
      return (static_cast<int64_t>(readInt32(buffer, offset)) << 32L) +
             static_cast<int64_t>(readUInt32(buffer, offset + 4));
    }
  }

  inline void writeUint64(uint8_t* buffer, uint64_t input,
                          size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<uint64_t*>(buffer + offset)[0] = input;
    } else {
      buffer[offset] = input & 0xFF;
      buffer[offset + 1] = (input >> 8) & 0xFF;
      buffer[offset + 2] = (input >> 16) & 0xFF;
      buffer[offset + 3] = (input >> 24) & 0xFF;
      buffer[offset + 4] = (input >> 32) & 0xFF;
      buffer[offset + 5] = (input >> 40) & 0xFF;
      buffer[offset + 6] = (input >> 48) & 0xFF;
      buffer[offset + 7] = (input >> 56);
    }
  }

  [[nodiscard]] inline uint64_t readUInt64(const uint8_t* buffer,
                                           size_t offset) const noexcept {
    if constexpr (littleEndian()) {
      return reinterpret_cast<const uint64_t*>(buffer + offset)[0];
    } else {
      return (static_cast<uint64_t>(readUInt32(buffer, offset)) << 32UL) +
             static_cast<uint64_t>(readUInt32(buffer, offset + 4));
    }
  }

  inline void writeDouble(uint8_t* buffer, double input,
                          size_t offset) noexcept {
    if constexpr (littleEndian()) {
      reinterpret_cast<double*>(buffer + offset)[0] = input;
    } else {
      doubleArray_[0] = input;
      const auto* temporary = reinterpret_cast<uint8_t*>(doubleArray_);
      buffer[offset] = temporary[7];
      buffer[offset + 1] = temporary[6];
      buffer[offset + 2] = temporary[5];
      buffer[offset + 3] = temporary[4];
      buffer[offset + 4] = temporary[3];
      buffer[offset + 5] = temporary[2];
      buffer[offset + 6] = temporary[1];
      buffer[offset + 7] = temporary[0];
    }
  }

  [[nodiscard]] inline double readDouble(const uint8_t* buffer,
                                         size_t offset) noexcept {
    if constexpr (littleEndian()) {
      return reinterpret_cast<const double*>(buffer + offset)[0];
    } else {
      auto* temporary = reinterpret_cast<uint8_t*>(doubleArray_);
      temporary[7] = buffer[offset];
      temporary[6] = buffer[offset + 1];
      temporary[5] = buffer[offset + 2];
      temporary[4] = buffer[offset + 3];
      temporary[3] = buffer[offset + 4];
      temporary[2] = buffer[offset + 5];
      temporary[1] = buffer[offset + 6];
      temporary[0] = buffer[offset + 7];
      return doubleArray_[0];
    }
  }

  inline void writeCString(uint8_t* buffer, const char* value, size_t size,
                           size_t offset) const noexcept {
    writeUint32(buffer, static_cast<uint32_t>(size), offset);
    memcpy(buffer + offset + sizeof(uint32_t), value, size);
  }

  inline void writeCString(uint8_t* buffer, const char* value,
                           size_t offset) const noexcept {
    writeCString(buffer, value, strlen(value), offset);
  }

  inline void writeString(uint8_t* buffer, const std::string& value,
                          size_t offset) const noexcept {
    writeCString(buffer, value.c_str(), value.length(), offset);
  }

  inline std::tuple<char*, size_t> readCString(const uint8_t* buffer,
                                               size_t offset) const noexcept {
    const auto size = readUInt32(buffer, offset);
    auto* value = reinterpret_cast<char*>(std::malloc(size * sizeof(char)));
    memcpy(value, buffer + offset + sizeof(uint32_t), size);
    return {value, size};
  }

  inline std::string readString(const uint8_t* buffer,
                                size_t offset) const noexcept {
    const auto& [value, size] = readCString(buffer, offset);
    return std::string(value, size);
  }
};
