#pragma once

#include "Core.h"
#include "sepch.h"

namespace se {

class Transform;
class Component;
class Entity;

class Object {
public:
    Object();
    Object(std::string);
    virtual ~Object();

    int InstanceID = 0;

    std::string name;

    std::string ToString();

    static void Destroy(Object*, float);
    static void Destroy(Object*);

    int GetInstanceID();

    bool operator==(const char*);
    bool operator==(const Object&);

    template <class T>
    bool is() {
        return dynamic_cast<T*>(this) != nullptr;
    }

    virtual void Copy(Object* other) {
        name = other->name;
    }

    virtual Object* Duplicate() {
        Object* pObj = new Object();
        pObj->Copy(this);
        return pObj;
    }
};

}  // namespace se