// Copyright (c) 2020 Antonio Román. All rights reserved.
#include "listeners/ContactListener.h"

#include "components/PhysicsBody.h"
#include "objects/GameObject.h"
#include "utils/DebugAssert.h"

void ContactListener::BeginContact(b2Contact* contact) {
  const auto& a =
      reinterpret_cast<PhysicsBody*>(contact->GetFixtureA()->GetUserData());
  const auto& b =
      reinterpret_cast<PhysicsBody*>(contact->GetFixtureB()->GetUserData());

  const auto ac = (a->category() & 0x0008) == 0x0008;
  const auto bc = (b->category() & 0x0008) == 0x0008;
  const auto collectible = ac || bc;

  debug_print("Colliding '%s' (c:%i) with '%s' (c:%i)! Collectible? %i\n",
              a->gameObject().lock()->name().c_str(), ac,
              b->gameObject().lock()->name().c_str(), bc, collectible);

  if (ac) a->destroy();
  if (bc) b->destroy();
}

void ContactListener::EndContact(b2Contact* contact) {
  const auto& a =
      reinterpret_cast<PhysicsBody*>(contact->GetFixtureA()->GetUserData());
  const auto& b =
      reinterpret_cast<PhysicsBody*>(contact->GetFixtureB()->GetUserData());

  debug_print("Not longer colliding '%s' with '%s'!\n",
              a->gameObject().lock()->name().c_str(),
              b->gameObject().lock()->name().c_str());
}
