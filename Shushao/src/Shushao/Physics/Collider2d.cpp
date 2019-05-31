#include "sepch.h"

#include <Box2D/Box2D.h>

#include "Shushao/Entity.h"
#include "Shushao/Sprite.h"
#include "Shushao/SpriteRenderer.h"
#include "Shushao/Transform.h"
#include "Collider2d.h"
#include "Rigidbody2d.h"

namespace se {

	//{ #region Collider2D (base virtual class)

	Collider2D::~Collider2D() {
		if (fixtureDef != nullptr) delete (b2FixtureDef*)fixtureDef;
		if (fixture != nullptr) delete (b2Fixture*)fixture;
	}

	void Collider2D::setup() {
		fixtureDef = new b2FixtureDef();
		((b2FixtureDef*)fixtureDef)->density = density;
		((b2FixtureDef*)fixtureDef)->friction = friction;
		((b2FixtureDef*)fixtureDef)->restitution = restitution;
		((b2FixtureDef*)fixtureDef)->isSensor = isSensor;
		((b2FixtureDef*)fixtureDef)->userData = this;
	}

	void Collider2D::OnDestroy() {
		/*if (fixture != nullptr && rigidbody != nullptr) {
			rigidbody->body->DestroyFixture(fixture);
		}*/
	}

	void Collider2D::ColliderEnter(Collider2D* other) {
		auto position = std::find(colliders.begin(), colliders.end(), other);
		if (position == colliders.end()) {
			colliders.push_back(other);
		}
	}

	void Collider2D::ColliderExit(Collider2D* other) {
		auto position = std::find(colliders.begin(), colliders.end(), other);
		if (position != colliders.end()) {
			colliders.erase(position);
		}
	}

	void Collider2D::SetDensity(float density_) {
		density = density_;
		((b2FixtureDef*)fixtureDef)->density = density;
		if (((b2Fixture*)fixture) != nullptr) {
			((b2Fixture*)fixture)->SetDensity(density);
		}
	}

	void Collider2D::SetRestitution(float restitution_) {
		restitution = restitution_;
		((b2FixtureDef*)fixtureDef)->restitution = restitution;
		if (((b2Fixture*)fixture) != nullptr) {
			((b2Fixture*)fixture)->SetRestitution(restitution);
		}
	}

	void Collider2D::SetFriction(float friction_) {
		friction = friction_;
		((b2FixtureDef*)fixtureDef)->friction = friction;
		if (((b2Fixture*)fixture) != nullptr) {
			((b2Fixture*)fixture)->SetFriction(friction);
		}
	}

	void Collider2D::SetSensor(bool sensor_) {
		isSensor = sensor_;
		((b2FixtureDef*)fixtureDef)->isSensor = isSensor;
		if (((b2Fixture*)fixture) != nullptr) {
			((b2Fixture*)fixture)->SetSensor(sensor_);
		}
	}

	void Collider2D::Awake() {
		scale = { transform->scale.x, transform->scale.y };
	}

	void Collider2D::Start() {
		Attach();
	}

	void Collider2D::FixedUpdate() {
		if (!colliders.empty()) {
			for (Collider2D* c : colliders) {
				entity->InvokeMethod("OnTriggerStay2D", *c);
			}
		}
	}

	void Collider2D::Attach() {
		rigidbody = entity->GetComponent<Rigidbody2D>();
		if (rigidbody == nullptr || rigidbody->GetBody() == nullptr) {
			return;
		}
		b2Body* bbo = (b2Body*)rigidbody->GetBody();
		fixture = bbo->CreateFixture((b2FixtureDef*)fixtureDef);
	}

	void Collider2D::Reset() {
		b2Body* bbo = (b2Body*)rigidbody->GetBody();
		bbo->DestroyFixture(((b2Fixture*)fixture));
		fixture = bbo->CreateFixture((b2FixtureDef*)fixtureDef);
	}

	//}

	//{ #region CircleCollider2D

	void CircleCollider2D::SetShape(glm::vec2 position_, float radius_) {
		position = position_;
		radius = radius_;

		SpriteRenderer* sr = entity->GetComponent<SpriteRenderer>();
		if (sr != nullptr) {
			((b2CircleShape*)shape)->m_p.Set(position_.x - sr->sprite->pivot.x * scale.x, position_.y - sr->sprite->pivot.y * scale.y);
		}
		else {
			((b2CircleShape*)shape)->m_p.Set(position_.x, position_.y);
		}
		((b2CircleShape*)shape)->m_radius = radius_ * scale.x;
		((b2FixtureDef*)fixtureDef)->shape = (b2CircleShape*)shape;
	}

	void CircleCollider2D::Copy(CircleCollider2D* other) {
		if (other == nullptr) return;
		Collider2D::Copy(other);

		*((b2CircleShape*)shape) = *(b2CircleShape*)other->shape;
		position = other->position;
		radius = other->radius;
	}
	void CircleCollider2D::ResetShape() {
		scale = { transform->scale.x, transform->scale.y };
		SetShape(position, radius);
	}

	//}

	//{ #region BoxCollider2D

	void BoxCollider2D::SetShape(glm::vec2 hsize) {
		SpriteRenderer* sr = entity->GetComponent<SpriteRenderer>();

		boxHalfSize = hsize;
		if (sr != nullptr) {
			b2Vec2 pivot(sr->sprite->pivot.x * scale.x, sr->sprite->pivot.y * scale.y);
			((b2PolygonShape*)shape)->SetAsBox(boxHalfSize.x * scale.x, boxHalfSize.y * scale.y, -pivot, 0.0f);
		}
		else {
			((b2PolygonShape*)shape)->SetAsBox(boxHalfSize.x * scale.x, boxHalfSize.y * scale.y);
		}
		((b2FixtureDef*)fixtureDef)->shape = (b2PolygonShape*)shape;
	}

	void BoxCollider2D::Copy(BoxCollider2D* other) {
		if (other == nullptr) return;
		Collider2D::Copy(other);

		*((b2PolygonShape*)shape) = *(b2PolygonShape*)other->shape;
		boxHalfSize = other->boxHalfSize;
	}

	void BoxCollider2D::ResetShape() {
		scale = { transform->scale.x, transform->scale.y };
		SetShape(boxHalfSize);
	}

	//}

	//{ #region EdgeCollider2D

	// TODO EdgeCollider2D

	//}

	//{ #region PolygonCollider2D

	// TODO PolygonCollider2D

	//}

}  // namespace se
