// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include "../exceptions/PointerException.h"

template <typename... Args>
void debug_print(Args... args) {
#if _DEBUG
  printf(args...);
#endif
}

template <typename Value>
void assert_not_null(Value value, const std::string& message) {
#if _DEBUG
  if (value == nullptr) throw PointerException(message);
#endif
}