#include "sepch.h"

#include "Component.h"
#include "Core.h"
#include "Debug.h"
#include "Scene.h"
#include "Transform.h"
#include "Entity.h"

namespace se {

	Entity::Entity() {
		name = "Entity";
		active = true;
		isStatic = false;
	}

	Entity::Entity(std::string _name) {
		name = _name;
		active = true;
		isStatic = false;
	}

	void Entity::init() {
		if (awaken) return;
		if (!registered && scene != nullptr) {
			scene->RegisterEntity(this);
		}
		transform = AddComponent<Transform>();
		Awake();
		awaken = true;
	}

	Entity::~Entity() {
		Debug::Log << "Entity Destructor: " << name << std::endl;

		// distruggo tutti i components
		for (Component* pCO : Components) {
			scene->UnregisterActiveComponent(pCO);
			delete (pCO);
		}

		scene->UnregisterEntity(this);
		scene->Invalidate();
		Components.clear();
	}

	void Entity::SetActive(bool value_) {  // Activates/Deactivates the Entity.
		active = value_;
		scene->Invalidate();
	}

	void Entity::Copy(Entity* other) {
		if (other == nullptr) return;
		Object::Copy(other);

		canvas = other->canvas;
		scene = other->scene;
		layer = other->layer;
		tag = other->tag;
		tags = other->tags;
		activeInHierarchy = other->activeInHierarchy;
		active = other->active;
		isStatic = other->isStatic;

		auto c = Components.begin();
		auto oc = other->Components.begin();
		for (; c != Components.end(); c++, oc++) {
			(*c)->Copy(*oc);
		}
	}

	void Entity::RegisterActiveComponent(Component* component) {
		scene->RegisterActiveComponent(component);
	}

	void Entity::UnregisterActiveComponent(Component* component) {
		scene->UnregisterActiveComponent(component);
	}

	void Entity::InvalidateScene() {
		scene->Invalidate();
	}

	Entity* Entity::AddChild(std::string _name = "Entity") {
		Entity* entity = new Entity();
		entity->transform->SetParent(transform);
		entity->name = _name;
		if (entity->scene != nullptr) {
			scene->Entities.insert(entity);
			entity->registered = true;
			entity->init();
		}
		return entity;
	}

	std::multiset<Component*, CompareComponent> Entity::GetActiveComponentsInChildren() {
		return transform->GetActiveComponentsInChildren();
	}

	void Entity::BroadcastInvokeMethod(std::string methodName) {
		/*		if (!isActiveInHierarchy())	return;

			for (Component* c : Components) {
				if (c->enabled)
					c->ReceiveMessage(methodName);
			}

			for (Transform* t : transform->children)
				t->entity->InvokeMethod(methodName);
	*/
	}

	void Entity::InvokeMethod(std::string methodName, Object& parameter) {
		if (!isActiveInHierarchy()) return;

		for (Component* c : Components) {
			if (c->enabled) c->ReceiveMessage(methodName, parameter);
		}
	}

	void Entity::PrintHierarchy(int level) {
		Logger::setColor(ConsoleColor::DARKGREEN);

		for (int i = 0; i < level; i++) std::cout << "   ";
		std::cout << " " << (char)192 << (char)196;
		std::cout << " " << name << (active ? "+" : "");

		for (Component* c : Components) std::cout << " [" << c->index << ". " << c->getTitle() << "]";

		std::cout << std::endl;

		++level;
		for (Transform* t : transform->children) {
			t->entity->PrintHierarchy(level);
		}

		Logger::setColor(ConsoleColor::WHITE);
	}

	bool Entity::isActiveInHierarchy() {
		if (!active) {
			return false;
		}

		Transform* p = transform->GetParent();
		while (p != nullptr) {
			if (!p->entity->active) {
				activeInHierarchy = false;
				return false;
			}
			p = p->GetParent();
		}

		return true;
	}

	void Entity::SetParent(Entity* other) {
		transform->SetParent(other->transform);
		canvas = other->canvas;
	}

	unsigned int Entity::GetNextIndex() {
		if (Components.size() > 0) {
			auto max = max_element(Components.begin(), Components.end(), [](const Component* a, const Component* b) {
				return a->index < b->index;
				});

			return (*max)->index + 1;
		}
		return 1;
	}

}  // namespace se
