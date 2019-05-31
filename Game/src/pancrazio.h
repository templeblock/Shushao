#pragma once

#include "globals.h"

class Pancrazio : public se::Entity {
	public:
		virtual void Awake();

		se::SpriteRenderer* sr;
		se::Rigidbody2D* rb;
		se::BoxCollider2D* collider;
		se::CircleCollider2D* circlec;
		se::Animation* animation;

};
