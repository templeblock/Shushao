#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

#include "Color.h"
#include "Material.h"
#include "Renderer.h"
#include "Shaders/Shader.h"
#include "Sprite.h"
#include "Texture.h"

namespace se {

class RawImage : public Renderer {
public:
    virtual void setup();
    ~RawImage();

    bool isReady();

    Texture* texture = nullptr;
    Material* material = nullptr;
    VertexArray* VAO = nullptr;

    Rect uvRect = {0, 0, 1, 1};
    Color color = color::white;

    RawImage* SetTexture(Texture* texture_) {
        texture = texture_;
        return this;
    }

    RawImage* SetUvRect(Rect rect_);
    RawImage* SetColor(Color color_);

    RawImage* SetMaterial(Material* material_) {
        material = material_;
        return this;
    }

    RawImage* Build();

    std::vector<glm::vec3> vertices{
        {-1.0f, -1.0f, 0.0f},  // Bottom-left
        {1.0f, -1.0f, 0.0f},  // Bottom-right
        {1.0f, 1.0f, 0.0f},  // Top-right
        {-1.0f, 1.0f, 0.0f}  // Top-left
    };

    std::vector<glm::vec3> normals{
        {0.0f, 0.0f, -1.0f}};

    std::vector<unsigned short int> indexes{
        0, 1, 2,
        2, 3, 0};

    std::vector<glm::vec2> uv{
        {0.0f, 1.0f},  // Bottom-left of texture
        {1.0f, 1.0f},  // Bottom-right of texture
        {1.0f, 0.0f},  // Top-Right of texture
        {0.0f, 0.0f}  // Top-left of texture
    };

protected:
    void Awake();
    void Update();
    void Render();
    void OnDestroy();
};

}  // namespace se
