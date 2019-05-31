#pragma once

#include "Core.h"
#include "CoroutineHolder.h"
#include "Object.h"
#include "sepch.h"

namespace se {

class Scene;
class Entity;
class Transform;
class Collision2D;
class Collider2D;

class CompareComponent {
public:
    bool operator()(Component* A, Component* B) const;
};

class Component : public Object, public CoroutineHolder {

    friend class Scene;
    friend class Entity;

public:
    Component();
    virtual ~Component();

    // va eseguita a mano dopo l'istanziazione
    virtual void setup() {};

    Entity* entity = nullptr;
    Transform* transform = nullptr;
    std::string tag;
    int sortingLayerID = 0;  // Unique ID of the Renderer's sorting layer.
    int sortingOrder = 0;  // Renderer's order within a sorting layer.
    unsigned int index = 0;  // index in entity

    bool awaken = false;

    inline bool operator<(Component B) {
        if (sortingLayerID == B.sortingLayerID) {
            return sortingOrder < B.sortingOrder;
        } else {
            return sortingLayerID < B.sortingLayerID;
        }
    }

    void Copy(Component* other);

    bool enabled = true;

    std::multiset<Component*, CompareComponent> GetActiveComponentsInChildren();
    std::set<Entity*> GetEntitiesInChildren();
    bool isActiveAndEnabled();

    // void run(Applications::Stage stage);
    virtual void call(std::string method);

    virtual void ReceiveMessage(std::string methodName, Object& parameter);
    virtual void ReceiveMessage(std::string methodName);

    void Enable();
    void Disable();

    std::string getTitle();

    //template <class T>
    //T* GetComponent(std::string _name = "");  // Returns the component of Type type if the game object has one attached, null if it doesn't.

    //template <class T>
    //T* AddComponent(std::string _name = "");  // Adds a component to the entity

protected:
    virtual void Awake(); 
    virtual void Start(); 
    virtual void Update();
    virtual void Render();
    virtual void FixedUpdate(); 
    virtual void LateUpdate();  
    virtual void OnPostRender(); 
    virtual void OnEnable(); 
    virtual void OnDestroy();
    virtual void OnDisable();

    // messages
    virtual void OnCollisionEnter2D(Collision2D&);
    virtual void OnCollisionExit2D(Collision2D&);
    virtual void OnCollisionStay2D(Collision2D&);
    virtual void OnTriggerEnter2D(Collider2D&);
    virtual void OnTriggerExit2D(Collider2D&);
    virtual void OnTriggerStay2D(Collider2D&);

private:
    bool currentEnable;
    bool started;

    void init();
    void update();
    void fixed();
    void render();
    void exit();
};

}  // namespace se
