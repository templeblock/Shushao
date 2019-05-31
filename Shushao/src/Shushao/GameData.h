#pragma once

#define BOOST_ALL_NO_LIB
#include <boost/variant.hpp>

#include "Core.h"
#include "sepch.h"
#include "Camera.h"
#include "Component.h"

namespace se {

using Multitype = boost::variant<int, float, double, std::string>;

class GameData {
public:
    static Camera* activeCamera;
    static Scene* scene;
    static std::vector<Object*> Objects;
    static std::vector<Component*> Components;

    static int GenerateObjectID();
    static int RegisterObject(Object*);

    static void UnRegisterObject(int);
    static void RegisterComponent(Component*);

    static void PrintAllObjects();
    static Object* GetObjectWithID(int);

    static void DestroyAll();

    template <class T>
    std::vector<T*> GetObjectsOfType() {
        std::vector<Object*> results;
        for (auto obj : Objects) {
            if (dynamic_cast<T*>(obj))
                results.push_back(dynamic_cast<T*>(obj));
        }
        return results;
    }

    Multitype operator[](std::string key) {
        return data[key];
    }

    static Multitype Get(std::string key) {
        return data[key];
    }

    static void Set(std::string key, Multitype value_) {
        data[key] = value_;
    }

private:
    static std::map<std::string, Multitype> data;
};

}  // namespace se
