#ifndef SE_SYSTEM
#define SE_SYSTEM

#include "Core.h"
#include "sepch.h"
#include "service.h"

namespace se {

class System {
public:
    static void init();
    static void update();
    static void exit();

    //static InputManager& Input;

    template <class T>
    static T& AddService() {
        T* service = new T();
        Services.push_back(service);
        return *service;
    }

    template <class T>
    static T& GetService() {
        for (Service* service : Services) {
            if (dynamic_cast<T*>(service))
                return dynamic_cast<T>(*service);
        }
        return nullptr;
    }

    static void Clear();

    static void ListServices();

protected:
    static std::vector<Service*> Services;

private:
};

}  // namespace se

#endif