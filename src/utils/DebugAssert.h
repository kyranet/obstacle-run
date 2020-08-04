// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#if _DEBUG
#define ASSERT_NOT_NULL(o, x) \
  if (o == nullptr) throw PointerException(x);
#define DEBUG_PRINT(fmt, ...) printf(fmt, __VA_ARGS__);
#else
#define ASSERT_NOT_NULL(o, x) {};
#define DEBUG_PRINT(fmt, ...) {};
#endif
