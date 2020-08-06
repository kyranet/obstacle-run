// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "utils/TimePool.h"

TimePool::TimePool(const uint32_t ticksInterval, const uint32_t now) noexcept
    : ticksInterval_(ticksInterval), ticksLast_(now) {}

bool TimePool::next(const uint32_t now) noexcept {
  ticksAccumulated_ += now - ticksLast_;
  ticksLast_ = now;

  if (ticksAccumulated_ >= ticksInterval_) {
    ticksAccumulated_ %= ticksInterval_;
    return true;
  }

  return false;
}

uint32_t TimePool::getRemaining() const noexcept {
  return ticksAccumulated_ > ticksInterval_
             ? 0
             : ticksInterval_ - ticksAccumulated_;
}
