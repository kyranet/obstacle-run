// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <json/json.h>

template <class T>
class JsonConvertible {
 public:
  virtual T fromJson(const Json::Value&) = 0;
  virtual Json::Value toJson(T value) = 0;
};
