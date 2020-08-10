// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "components/PhysicsBody.h"

#include "Game.h"
#include "objects/GameObject.h"
#include "objects/PhysicsWorld.h"
#include "scenes/Scene.h"

PhysicsBody::PhysicsBody(std::weak_ptr<GameObject> parent,
                         Vector4<int32_t> vector) noexcept
    : Component(std::move(parent)) {
  id_ = gameObject().lock()->scene().lock()->physics()->add(vector);
}

PhysicsBody::~PhysicsBody() noexcept {
  const auto& go = gameObject().lock();
  if (!go) return;

  const auto& scene = go->scene().lock();
  scene->physics()->remove(id_);
}

const Vector4<int32_t>& PhysicsBody::data() const noexcept {
  return gameObject().lock()->scene().lock()->physics()->at(id_);
}

#if !NDEBUG
void PhysicsBody::onRender() noexcept {
  Component::onRender();

  const auto destination = data().toRectangle();
  SDL_SetRenderDrawColor(Game::renderer(), 255, 0, 0, 100);
  SDL_RenderDrawRect(Game::renderer(), &destination);
}
#endif
