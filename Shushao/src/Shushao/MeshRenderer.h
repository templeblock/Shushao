#pragma once

#include "Color.h"
#include "Core.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shaders/Shader.h"
#include "sepch.h"

namespace se {

class MeshRenderer : public Renderer {
public:
    virtual void setup();
    ~MeshRenderer();

    bool isReady();

    Mesh* mesh = nullptr;
    Material* material = nullptr;

    MeshRenderer* SetMesh(Mesh* mesh_) {
        mesh = mesh_;
        return this;
    }

    MeshRenderer* SetMaterial(Material* material_) {
        material = material_;
        return this;
    }

protected:
    void Awake();
    void Update();
    void Render();
    void OnDestroy();

private:
};

}  // namespace se
