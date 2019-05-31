#include "meshtest.h"

using namespace se;

void Meshtest::Awake() {
    Entity* directional = AddChild("Directional");
    directional->transform->SetLocalRotation({20, 10, 0});
    Light* light = directional->AddComponent<Light>();
    light->type = Light::Type::DIRECTIONAL;
    light->ambient = {0.2f, 0.2f, 0.2f};
    light->diffuse = {1.0f, 0.5f, 0.5f};
    light->specular = {1.0f, 1.0f, 1.0f};

    Entity* point = AddChild("Point 1");
    point->transform->SetLocalPosition({3.0f, 1.7f, 4.0f});
    Light* plight = point->AddComponent<Light>();
    plight->type = Light::Type::POINT;
    plight->ambient = {0.3f, 0.3f, 0.3f};
    plight->diffuse = {0.7f, 0.7f, 0.7f};
    plight->specular = {1.0f, 1.0f, 1.0f};
    plight->attenuation = {0.6f, 0.09f, 0.012f};

    MeshRenderer* mr = point->AddComponent<MeshRenderer>();
    mr->SetMesh(Resources::Get<Mesh>("cube"));
    mr->material->SetColor("ambient_color", color::white);
    mr->material->SetColor("diffuse_color", color::white);
    mr->material->SetColor("specular_color", color::white);
    mr->material->SetFloat("shininess", 20.0f);
    mr->transform->SetLocalScale({0.1f, 0.1f, 0.1f});

    AddChild<Starship>("Cassa");
    Entity* mah = AddChild<Starship>("mah");
    mah->transform->SetLocalPosition({4.0f, 2.0f, -1.0f});
    mah->transform->SetLocalRotation({10, 10, 0});
}