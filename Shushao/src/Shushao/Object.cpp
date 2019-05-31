#include "sepch.h"

#include "Debug.h"
#include "Utility.h"
#include "Component.h"
#include "GameData.h"
#include "SceneManager.h"

namespace se {

	Object::Object() {
		name = "Object";
		GameData::RegisterObject(this);
	}

	Object::Object(std::string _name) {
		name = _name;
		GameData::RegisterObject(this);
	}

	Object::~Object() {
		Destroy(this);
	}

	bool Object::operator==(const char* _name) {
		return name == _name;
	}

	bool Object::operator==(const Object& obj) {
		return obj.InstanceID == InstanceID;
	}

	std::string Object::ToString() {
		return name;
	}

	int Object::GetInstanceID() {
		return InstanceID;
	}

	void Object::Destroy(Object* obj, float t) {
		// TODO: GameData::UnRegisterObject(this.InstanceID);

		// TODO: eliminazione ritardata con t

		// delete obj;
	}

	void Object::Destroy(Object* obj) {
		// TODO: eliminazione ritardata con t

		// delete obj;
	}

	/* 	TODO: Clones the object original and returns the clone.

				This function makes a copy of an object in a similar way to the Duplicate command in the editor. If you are cloning a Entity then you can also optionally
				specify its position and rotation (these default to the original Entity's position and rotation otherwise). If you are cloning a Component then the Entity
				it is attached to will also be cloned, again with an optional position and rotation.

				When you clone a Entity or Component, all child objects and components will also be cloned with their properties set like those of the original object.

				By default the parent of the new object will be null, so it will not be a "sibling" of the original. However, you can still set the parent using the overloaded
				methods. If a parent is specified and no position and rotation is specified, the position and rotation of the original will be used for the cloned object's local
				position and rotation, or its world position and rotation if the instantiateInWorldSpace parameter is true. If the position and rotation is specified, they will
				be used as the object's position and rotation in world space.

				The active status of a Entity at the time of cloning will be passed on, so if the original is inactive then the clone will be created in an inactive state too.
			*/

			/* Object* Object::Instantiate(Object* original, Transform* parent) {
					// TODO Object::Instantiate
					return nullptr;
				}

				Object* Object::Instantiate(Object* original, Transform* parent, bool instantiateInWorldSpace) {
					// TODO Object::Instantiate
					return nullptr;
				}

				Object* Object::Instantiate(Object* original, glm::vec3 position, quat rotation) {
					// TODO Object::Instantiate
					return nullptr;
				}

				Object* Object::Instantiate(Object* original, glm::vec3 position, quat rotation, Transform* parent) {
					// TODO Object::Instantiate
					return nullptr;
				} */

}  // namespace se
