// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/SolidRendererFactory.h"

#include <cassert>

SolidRendererFactory::SolidRendererFactory() noexcept
    : ComponentFactory("SolidRenderer") {}

SolidRendererFactory::~SolidRendererFactory() noexcept = default;
