// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include "exceptions/PointerException.h"

template <typename... Args>
#if NDEBUG
void debug_print(Args...) {
#else
void debug_print(Args... args) {
  printf(args...);
#endif
}

template <typename Value>
#if NDEBUG
void assert_not_null(Value, const std::string&) {
#else
void assert_not_null(Value value, const std::string& message) {
  if (value == nullptr) throw PointerException(message);
#endif
}