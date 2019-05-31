#include "sepch.h"

#include <Box2D/Box2D.h>

#include "Physics.h"
#include "PhysicsContactFilter.h"
#include "PhysicsContactListener.h"
#include "PhysicsDebugDraw.h"
#include "Shushao/Config.h"
#include "Shushao/SceneManager.h"

namespace se {

	// initializations
	bool Physics::enabled;
	bool Physics::debug;
	float32 Physics::timeStep;
	bool Physics::doSleep;
	int32 Physics::velocityIterations;
	int32 Physics::positionIterations;

	PhysicsDebugDraw debugDraw;
	PhysicsContactListener contactListener;
	PhysicsContactFilter contactFilter;

	class Physics::Impl {
	public:
		~Impl() {
			if (world == nullptr) return;
			delete world;
		}
		b2World* world;
		b2Vec2 gravity;
	};

	Pimpl<Physics::Impl> Physics::impl = spimpl::make_impl<Impl>();

	bool Physics::Init() {

		//impl = spimpl::make_impl<Impl>();

		enabled = Config::Physics::enabled;
		debug = Config::Physics::debug;
		impl->gravity = { Config::Physics::gravity.x, Config::Physics::gravity.y };
		timeStep = 1.0f / Config::Time::fixedRateLimit;
		doSleep = Config::Physics::doSleep;
		velocityIterations = Config::Physics::velocityIterations;
		positionIterations = Config::Physics::positionIterations;

		impl->world = new b2World(impl->gravity);
		impl->world->SetAllowSleeping(doSleep);
		impl->world->SetContactListener(&contactListener);
		impl->world->SetContactFilter(&contactFilter);

		if (debug) {
			impl->world->SetDebugDraw(&debugDraw);
			debugDraw.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
			/*	e_shapeBit ( draw shapes )
				 *	e_jointBit ( draw joint connections
				 *	e_aabbBit ( draw axis aligned bounding boxes )
				 *	e_pairBit ( draw broad-phase pairs )
				 *	e_centerOfMassBit ( draw a marker at body CoM )
				 */
			debugDraw.Init();
		}

		return true;
	}

	void Physics::SetGravity(glm::vec3 gravity_) {
		if (!enabled) return;
		if (impl->world == nullptr) return;
		impl->gravity = b2Vec2(gravity_.x, gravity_.y);
		impl->world->SetGravity(impl->gravity);
	}

	void* Physics::GetWorld() {
		return (b2World*)impl->world;
	}

	void* Physics::GetGravity() {
		return (b2Vec2*)(&(impl->gravity));
	}

	void Physics::Update() {
		if (!enabled) return;
		if (impl->world == nullptr) return;
		impl->world->Step(timeStep, velocityIterations, positionIterations);
	}

	void Physics::Exit() {
	}

}  // namespace se
