#include "sepch.h"

#include "Application.h"
#include "Core.h"
#include "Debug.h"
#include "DebugInfo.h"
#include "UI/CanvasUI.h"
#include "Scene.h"

namespace se {

Scene::Scene() {
    name = "Scene";

    // root va istanziata manualmente a questo livello
    root = new Entity("ROOT");
    root->scene = this;
    root->transform = root->AddComponent<Transform>();
    root->transform->isRoot = true;
    root->awaken = true;
    RegisterEntity(root);

    MainCamera* mainCameraObj = AddEntity<MainCamera>();
    activeCamera = mainCameraObj->camera;
    GameData::activeCamera = activeCamera;

    if (Debug::enabled) {
        if (Debug::gridEnabled) {
            AddEntity<DebugGrid>("Debug Grid");
        }

        if (Debug::infoEnabled) {
            ui::CanvasUI* di = AddEntity<ui::CanvasUI>("Debug Info");
            di->canvas->SetRenderMode(RenderMode::SCREEN);
            debugInfo = di->AddComponent<DebugInfo>();
        }
    }
}

Scene::~Scene() {
    std::vector<Entity*> toDelete;
    for (Entity* pGO : Entities) {
        toDelete.push_back(pGO);
    }
    for (Entity* pGO : toDelete) {
        delete (pGO);
    }
    Entities.clear();
}

bool Scene::HasEntity(Entity* entity_) {
    return Entities.find(entity_) != Entities.end();
}

Entity* Scene::AddEntity(std::string _name = "Entity") {
    Entity* entity = new Entity();
    entity->name = _name;
    entity->scene = this;
    entity->init();
    entity->transform->SetParent(root->transform);
    RegisterEntity(entity);
    return entity;
}

Entity* Scene::GetEntity(std::string name_) {
    for (Entity* entity : Entities) {
        if (entity->name == name_)
            return entity;
    }
    return nullptr;
}

void Scene::ScanActiveComponents() {
    ActiveComponents.clear();
    ActiveOverlayRenderers.clear();

    // ottengo i component attivi
    ActiveComponents = root->transform->GetActiveComponentsInChildren();

    for (Component* component : ActiveComponents) {
        Renderer* renderer = dynamic_cast<Renderer*>(component);
        if (renderer != nullptr && renderer->overlay)
            ActiveOverlayRenderers.insert(component);
    }

    ActiveLights.clear();
    for (Component* component : ActiveComponents) {
        if (dynamic_cast<Light*>(component)) {
            ActiveLights.insert((Light*)component);
        }
    }
    invalid = false;
}

void Scene::ScanEntities() {
    // ottengo tutte le entities, anche child
    std::set<Entity*> entities_ = root->transform->GetEntitiesInChildren();

    for (auto& entity : entities_) {
        if (!entity->awaken) {
            entity->init();
        }
        if (!entity->registered) {
            RegisterEntity(entity);
        }
    }
    invalid = false;
}

void Scene::PrintActiveComponentsInScene() {
    ScanActiveComponents();
    if (!Debug::enabled) return;
    Logger::setColor(ConsoleColor::DARKCYAN);
    std::cout << " Scene " << name << " Active Components:" << std::endl;
    for (Component* component : ActiveComponents) {
        std::cout << "  - " << component->getTitle() << " (" << component->entity->name << ")" << std::endl;
    }
    Logger::setColor(ConsoleColor::LIGHTGREY);
}

void Scene::PrintActiveLightsInScene() {
    if (!Debug::enabled) return;
    Logger::setColor(ConsoleColor::PINK);
    std::cout << " Scene " << name << " Active Lights:" << std::endl;
    for (Light* light : ActiveLights) {
        std::cout << "  - " << light->GetTypeDesc() << " light " << light->getTitle() << " (" << light->entity->name << ")" << std::endl;
    }
    Logger::setColor(ConsoleColor::LIGHTGREY);
}

void Scene::PrintActiveRenderersInScene() {
    ScanActiveComponents();
    if (!Debug::enabled) return;
    Logger::setColor(ConsoleColor::DARKPURPLE);
    std::cout << " Scene " << name << " Active Renderers:" << std::endl;
    for (Component* component : ActiveComponents) {
        if (!dynamic_cast<Renderer*>(component)) continue;
        int layerID = ((Renderer*)component)->sortingLayerID;
        std::cout << "  - "
                  << "[" << Config::SortingLayers[layerID] << " (" << layerID << ")"
                  << ", " << component->sortingOrder << "] " << component->getTitle() << " (" << component->entity->name << ")" << std::endl;
    }
    std::cout << " Scene " << name << " Active Overlay Renderers:" << std::endl;
    for (Component* component : ActiveOverlayRenderers) {
        if (!dynamic_cast<Renderer*>(component)) continue;
        int layerID = ((Renderer*)component)->sortingLayerID;
        std::cout << "  - "
                  << "[" << Config::SortingLayers[layerID] << " (" << layerID << ")"
                  << ", " << component->sortingOrder << "] " << component->getTitle() << " (" << component->entity->name << ")" << std::endl;
    }
    Logger::setColor(ConsoleColor::LIGHTGREY);
}

void Scene::init() {
    for (Component* component : ActiveComponents) {
        if (component != nullptr && !component->awaken) component->init();
    }
}

void Scene::update() {
    for (Component* component : ActiveComponents) {
        if (component != nullptr) component->update();
    }
}

void Scene::fixed() {
    for (Component* component : ActiveComponents) {
        if (component != nullptr) component->fixed();
    }
}

void Scene::render() {
    for (Component* component : ActiveComponents) {
        Renderer* renderer = dynamic_cast<Renderer*>(component);
        if (renderer != nullptr && !renderer->overlay) {  // non eseguo il rendering degli overlay in questo ciclo
            component->render();
        } else if (renderer == nullptr && component != nullptr) {
            component->render();
        }
    }
}

void Scene::exit() {
    for (Component* component : ActiveComponents) {
        if (component != nullptr) component->exit();
    }
}

void Scene::renderOverlay() {
    for (Component* component : ActiveOverlayRenderers) {
        if (component != nullptr) component->render();
    }
}

void Scene::PrintHierarchy() {
    if (!Debug::enabled) return;
    Logger::setColor(ConsoleColor::GREEN);
    std::cout << " Scene " << name << "" << std::endl;
    root->PrintHierarchy(0);
    Logger::setColor(ConsoleColor::LIGHTGREY);
}

std::multiset<Entity*> Scene::GetRootEntitys() {
    // TODO: I GAMEOBJECT CON PARENT ROOT
    return Entities;
}

Entity* Scene::RegisterEntity(Entity* entity_) {
    if (entity_ == nullptr) return nullptr;

    if (Entities.empty() || !HasEntity(entity_)) {
        entity_->scene = this;
        Entities.insert(entity_);
        entity_->registered = true;
    }

    return entity_;
}

void Scene::UnregisterEntity(Entity* entity_) {
    if (HasEntity(entity_)) {
        Entities.erase(entity_);
    }
}

void Scene::UnregisterActiveComponent(Component* component) {
    if (ActiveComponents.empty()) {
        return;
    }

    auto it = ActiveComponents.find(component);
    if (it != ActiveComponents.end()) {
        ActiveComponents.erase(it);
    }

    auto it2 = ActiveOverlayRenderers.find(component);
    if (it2 != ActiveOverlayRenderers.end()) {
        ActiveOverlayRenderers.erase(it2);
    }

    auto it3 = ActiveLights.find((Light*)component);
    if (it3 != ActiveLights.end()) {
        ActiveLights.erase(it3);
    }
}

void Scene::RegisterActiveComponent(Component* component) {
    if (!ActiveComponents.empty()) {
        auto it = ActiveComponents.find(component);
        if (it != ActiveComponents.end()) {
            return;
        }
    }

    ActiveComponents.insert(component);

    Renderer* renderer = dynamic_cast<Renderer*>(component);
    if (renderer != nullptr && renderer->overlay) {
        ActiveOverlayRenderers.insert(component);
    }

    if (dynamic_cast<Light*>(component)) {
        ActiveLights.insert((Light*)component);
    }
}

}  // namespace se
