// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "factories/ImageRendererFactory.h"

#include <cassert>

ImageRendererFactory::ImageRendererFactory() noexcept
    : ComponentFactory("ImageRenderer") {}

ImageRendererFactory::~ImageRendererFactory() noexcept = default;
