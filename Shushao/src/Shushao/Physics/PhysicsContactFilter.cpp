#include "sepch.h"

#include <Box2D/Box2D.h>

#include "Collider2d.h"
#include "PhysicsContactFilter.h"
#include "Shushao/Entity.h"
#include "Shushao/transform.h"

namespace se {

	bool PhysicsContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) {
		Entity* A = ((Collider2D*)fixtureA->GetUserData())->entity;
		Entity* B = ((Collider2D*)fixtureB->GetUserData())->entity;

		if (A == B) return false;
		if (A->transform->GetParent() == B->transform) return false;
		if (B->transform->GetParent() == A->transform) return false;

		return true;
	}

}  // namespace se
