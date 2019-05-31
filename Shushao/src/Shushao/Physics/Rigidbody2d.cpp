#include "sepch.h"

#include <Box2D/Box2D.h>

#include "Physics.h"
#include "Rigidbody2d.h"
#include "Shushao/Entity.h"
#include "Shushao/transform.h"

namespace se {

	struct Rigidbody2D::Impl {
		b2Body* body = nullptr;
		b2BodyDef bodyDef;
	};

	PLIMPL(Rigidbody2D)

	// Plimpl constructor
	// Rigidbody2D::Rigidbody2D(): impl(spimpl::make_impl<Impl>()) {}

	void Rigidbody2D::Copy(Rigidbody2D* other) {
		if (other == nullptr) return;
		Component::Copy(other);

		position = other->position;
		velocity = other->velocity;
		angle = other->angle;
		angularVelocity = other->angularVelocity;
		fixedRotation = other->fixedRotation;
		type = other->type;  // TODO: controllare che sia copy
	}

	void* Rigidbody2D::GetBody() {
		return (b2Body*)impl->body;
	}
	void* Rigidbody2D::GetBodyDef() {
		return (b2BodyDef*) & (impl->bodyDef);
	}

	void Rigidbody2D::OnDestroy() {
		if (impl->body == nullptr) return;
		((b2World*)Physics::GetWorld())->DestroyBody(impl->body);
	}

	void Rigidbody2D::Awake() {
		position = transform->position;
		angle = transform->GetEulerAngles().z;

		/// DA CAPIRE, se devo lasciare dynamic (fa le collisioni con gli static) o no
		/*if (!transform->isAtRoot() && type == RigidbodyType::DYNAMIC) {
				type = RigidbodyType::KINEMATIC;
			}*/

		impl->bodyDef.type = (b2BodyType)type;
		impl->bodyDef.position.Set(position.x, position.y);
		impl->bodyDef.angle = angle * DEGTORAD;  // RADIANS
		impl->bodyDef.fixedRotation = fixedRotation;
		impl->bodyDef.userData = this;
		impl->bodyDef.active = enabled && entity->active;

		impl->body = ((b2World*)Physics::GetWorld())->CreateBody(&(impl->bodyDef));
	}

	void Rigidbody2D::OnEnable() {
		impl->body->SetActive(transform->isAtRoot() && entity->active);
	}

	void Rigidbody2D::OnDisable() {
		impl->body->SetActive(false);
	}

	void Rigidbody2D::SetType(RigidbodyType type_) {
		type = type_;
		impl->bodyDef.type = (b2BodyType)type;
		if (impl->body != nullptr) {
			impl->body->SetType(impl->bodyDef.type);
		}
	}

	void Rigidbody2D::SetFixedRotation(bool fr) {
		fixedRotation = fr;
		impl->bodyDef.fixedRotation = fr;
		if (impl->body != nullptr) {
			impl->body->SetFixedRotation(fr);
		}
	}

	void Rigidbody2D::Update() {
		if (transform->isAtRoot()) {
			impl->body->SetActive(entity->active);
		}
	}

	void Rigidbody2D::FixedUpdate() {
		if (type == RigidbodyType::STATIC) {
			position = transform->position;
			angle = transform->GetEulerAngles().z;  // DEGREES
			impl->body->SetTransform(b2Vec2(position.x, position.y), angle * DEGTORAD);
			return;
		}

		b2Vec2 p = impl->body->GetPosition();
		b2Vec2 v = impl->body->GetLinearVelocity();
		position = { p.x, p.y };
		velocity = { v.x, v.y };
		angle = impl->body->GetAngle() * RADTODEG;  // DEGREES
		angularVelocity = impl->body->GetAngularVelocity();

		transform->SetPosition({ position.x, position.y, transform->position.z });
		transform->SetRotation({ 0.0f, 0.0f, angle });
	}
}  // namespace se
