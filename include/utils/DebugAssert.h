// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include "exceptions/PointerException.h"

template <typename... Args>
#if NDEBUG
void debug_print(Args...) {}
#else
void debug_print(const char* fmt, Args... args) {
  printf(fmt, args...);
}
#endif
