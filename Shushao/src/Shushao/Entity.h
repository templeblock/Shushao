#pragma once

#include "Component.h"
#include "Core.h"
#include "Object.h"
#include "Renderer.h"
#include "Utility.h"
#include "canvas.h"
#include "sepch.h"

namespace se {

class Scene;
class Transform;

class Entity : public Object {
    friend class Scene;

public:
    Entity();
    Entity(std::string);

    ~Entity();

    std::multiset<Component*, CompareComponent> Components;

    Transform* transform = nullptr;
    Canvas* canvas = nullptr;
    Scene* scene = nullptr;  // Scene that the Entity is part of.
    bool registered = false;
    bool awaken = false;

    int layer;  // The layer the game object is in. A layer is in the range [0...31].
    std::string tag;  // The tag of this game object.
    std::set<std::string> tags;  // The tag of this game object.

    bool activeInHierarchy;  // Is the Entity active in the scene?
    bool active = true;  // The local active state of this Entity. (Read Only)
    bool isStatic;  // 	Editor only API that specifies if a game object is static.
    std::multiset<Component*, CompareComponent> GetActiveComponentsInChildren();
    void PrintHierarchy(int);
    bool isActiveInHierarchy();

    void BroadcastInvokeMethod(std::string methodName);  // Calls the method named methodName on every MonoBehaviour in this game object or any of its children.
    void InvokeMethod(std::string methodName, Object& parameter);  // Calls the method named methodName on every MonoBehaviour in this game object.
    void InvokeMethodUpwards(std::string methodName);  // Calls the method named methodName on every MonoBehaviour in this game object and on every ancestor of the behaviour.

    bool CompareTag(std::string t);
    void SetParent(Entity*);
    Entity* AddChild(std::string);

    void Copy(Entity* other);

    void RegisterActiveComponent(Component* component);
    void UnregisterActiveComponent(Component* component);
    void InvalidateScene();

    template <class T>
    T* AddComponent(std::string _name = "") {
        T* component = new T();

        component->name = (_name == "" ? util::classtitle<T>() : _name);
        component->transform = transform;
        component->entity = this;
        component->setup();  // va sempre chiamato
        component->index = GetNextIndex();

        // tratto i componenti se siamo in una canvas
        if (GetComponent<Canvas>() != nullptr) {
            if (dynamic_cast<Renderer*>(component)) {
                ((Renderer*)component)->overlay = true;
            }
        }

        Components.insert(component);
        if (active) {
            RegisterActiveComponent(component);
        }
        return (T*)component;
    }

    template <class T>
    T* AddProgram(std::string _name = "") {  // Adds a component class named className to the game object.
        T* component = new T();
        component->name = (_name == "" ? util::classtitle<T>() : _name);
        component->transform = transform;
        component->entity = this;
        component->setup();  // va sempre chiamato
        component->index = GetNextIndex();

        // tratto i componenti se siamo in una canvas
        if (GetComponent<Canvas>() != nullptr) {
            if (dynamic_cast<Renderer*>(component)) {
                ((Renderer*)component)->overlay = true;
            }
        }

        Components.insert(component);
        if (component->isActiveAndEnabled()) {
            RegisterActiveComponent(component);
        }
        return component;
    }

    template <class T>
    T* AddChild(std::string _name = "") {  // Adds a Entity of class T as a child
        T* entity = new T();
        entity->name = (_name == "" ? util::classtitle<T>() : _name);
        entity->scene = scene;
        if (entity->scene != nullptr) {
            entity->init();
        }
        entity->transform->SetParent(transform);
        for (Component* c : entity->Components) {
            if (c->isActiveAndEnabled()) {
                RegisterActiveComponent(c);
            }
        }
        return entity;
    }

    template <class T>
    T* GetComponent(std::string _name = "") {  // Returns the component of Type type if the game object has one attached, null if it doesn't.
        for (Component* component : Components)
            if (dynamic_cast<T*>(component)) {
                if (_name == "" || component->name == _name) return dynamic_cast<T*>(component);
            }
        return nullptr;
    }

    Component* GetComponent(std::string _name) {  // Returns the component of name
        for (Component* component : Components)
            if (component->name == _name) return component;
        return nullptr;
    }

    Component* GetComponent(unsigned int index_) {  // Returns the component of name
        for (Component* component : Components)
            if (component->index == index_) return component;
        return nullptr;
    }

    template <class T>
    T* GetComponentInChildren();  // TODO: Returns the component of Type type in the GameObject or any of its children using depth first search.

    template <class T>
    T* GetComponentInParent();  // TODO: Returns the component of Type type in the Entity or any of its parents.

    template <class T>
    T* GetComponents();  // TODO: Returns all components of Type type in the Entity.

    template <class T>
    T* GetComponentsInChildren();  // TODO: Returns all components of Type type in the Entity or any of its children.

    template <class T>
    T* GetComponentsInParent();  // TODO: Returns all components of Type type in the Entity or any of its parents.

    void SetActive(bool value_);

    static Entity* Find(std::string);  // Finds a Entity by name and returns it.
    static Entity* FindEntitysWithTag(std::string);  // Returns a list of active Entitys tagged tag. Returns empty array if no Entity was found.
    static Entity* FindWithTag(std::string);  //	Returns one active Entity tagged tag. Returns null if no Entity was found.

    unsigned int GetNextIndex();

protected:
    virtual void Awake() {}

private:
    void init();
};

}  // namespace se
