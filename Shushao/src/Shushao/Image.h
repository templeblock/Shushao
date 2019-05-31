#pragma once

#include "Color.h"
#include "Core.h"
#include "Material.h"
#include "Renderer.h"
#include "Shaders/Shader.h"
#include "Sprite.h"
#include "sepch.h"

namespace se {

class Image : public Renderer {
public:
    enum class Type {
        SIMPLE,  // Displays the full Image
        SLICED,  // Displays the Image as a 9-sliced graphic.
        TILED,  // Displays a sliced Sprite with its resizable sections tiled instead of stretched
        FILLED  // Displays only a portion of the Image
    };

    virtual void setup();
    ~Image();

    bool isReady();

    Sprite* sprite = nullptr;
    Material* material = nullptr;
    VertexArray* VAO = nullptr;

    Image::Type type = Image::Type::SIMPLE;
    glm::vec4 border;  // pixel borders for 9-slicing - X=left, Y=bottom, Z=right, W=top

    bool fillCenter = true;
    bool preserveAspect = false;

    Image* SetBorder(glm::vec4 border_);
    Image* SetFillCenter(bool value_);
    Image* SetSprite(Sprite* sprite_);
    Image* SetImageType(Image::Type type_);
    Image* SetPreserveAspect(bool value_);
    Image* SetMaterial(Material* material_);

    Color color = {1.0f, 1.0f, 1.0f, 1.0f};  //	Rendering color for the Sprite graphic.
    bool flipX;  //	Flips the sprite on the X axis.
    bool flipY;  //	Flips the sprite on the Y axis.

protected:
    void Awake();
    void Update();
    void Render();
    void OnDestroy();

private:
    void refreshSprite();
    glm::vec2 last_rectSize;
};

}  // namespace se
