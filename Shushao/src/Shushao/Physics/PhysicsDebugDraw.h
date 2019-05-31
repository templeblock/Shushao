#pragma once

#include <Box2D/Box2D.h>

#include "Shushao/Core.h"
#include "sepch.h"

#include "Shushao/VertexArray.h"
#include "Shushao/shaders/Shader.h"

namespace se {

class PhysicsDebugDraw : public b2Draw {
public:
    bool Init();
    float alpha = 0.4f;
    VertexArray* VAO;
    Shader* shader = nullptr;
    bool ready = false;

    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
    virtual void DrawTransform(const b2Transform& xf) override;
    virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override {}
};

}  // namespace se
