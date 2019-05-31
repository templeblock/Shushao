#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

#include "Component.h"
#include "Entity.h"
#include "Scene.h"

namespace se {

class Factory {
public:
    template <class T, class P>
    static P* InstantiateComponent(P* originalComponent, Transform* parent = nullptr, bool instantiateInWorldSpace = false) {
        T* newEntity = ((Component*)originalComponent)->entity->scene->AddEntity<T>();
        P* newComponent = (P*)((Entity*)newEntity)->GetComponent(originalComponent->index);
        newEntity->Copy(originalComponent->entity);
        newComponent->Copy(originalComponent);
        if (parent != nullptr) {
            ((Entity*)newEntity)->SetParent(originalComponent->entity);
        }
        if (instantiateInWorldSpace) {
            ((Entity*)newEntity)->transform->SetPosition(originalComponent->entity->transform->position);
        }
        ((Component*)newComponent)->entity->name = ((Component*)originalComponent)->entity->name + " Copy";
        return (P*)newComponent;
        return nullptr;
    }

    template <class T, class P>
    static P* InstantiateComponent(P* originalComponent, glm::vec3 position, glm::quat rotation = Transform::QUATERNION_IDENTITY, Transform* parent = nullptr) {
        P* newComponent = InstantiateComponent<T>(originalComponent, parent);
        ((Component*)newComponent)->transform->SetLocalPosition(position);
        if (rotation != Transform::QUATERNION_IDENTITY) {
            ((Component*)newComponent)->entity->transform->SetLocalRotation(rotation);
        }
        return (P*)newComponent;
        return nullptr;
    }

    template <class T>
    static T* InstantiateEntity(T* originalEntity, Transform* parent = nullptr, bool instantiateInWorldSpace = false) {
        T* newEntity = ((Entity*)originalEntity)->scene->AddEntity<T>();

        newEntity->Copy(originalEntity);
        newEntity->SetActive(originalEntity->active);
        newEntity->SetParent(originalEntity);
        if (parent != nullptr) {
            ((Entity*)newEntity)->transform->SetParent(parent);
        }
        if (instantiateInWorldSpace) {
            ((Entity*)newEntity)->transform->SetPosition(originalEntity->transform->position);
        }
        ((Entity*)newEntity)->name = originalEntity->name + " Copy";
        return (T*)newEntity;
    }

    template <class T>
    static T* InstantiateEntity(T* originalEntity, glm::vec3 position, glm::quat rotation = Transform::QUATERNION_IDENTITY, Transform* parent = nullptr) {
        T* newEntity = InstantiateEntity(originalEntity, parent);
        ((Entity*)newEntity)->transform->SetLocalPosition(position);
        if (rotation != Transform::QUATERNION_IDENTITY) {
            ((Entity*)newEntity)->transform->SetLocalRotation(rotation);
        }
        return (T*)newEntity;
    }
};

}  // namespace se
