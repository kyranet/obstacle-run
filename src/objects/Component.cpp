// Copyright (c) 2020 Antonio Román. All rights reserved.
#include <objects/Component.h>

#include "objects/GameObject.h"

Component::Component() noexcept = default;
Component::Component(GameObject* gameObject) noexcept
    : gameObject_(gameObject) {}
Component::~Component() noexcept = default;

void Component::destroy() noexcept {}

void Component::onAwake() noexcept { enabled() = true; }
void Component::onUpdate() noexcept {}
