#pragma once

#include "Color.h"
#include "Core.h"
#include "Rect.h"
#include "Shaders/Shader.h"
#include "Transform.h"
#include "VertexArray.h"
#include "sepch.h"

namespace se {

enum class DrawMode {
    HOLLOW,
    FULL,
    BORDERED
};

typedef struct {
    int id;
    float duration;
} Expiration;

class Design {
public:
    static void DrawPoint(glm::vec3 position, Color color, int tickness, RenderMode renderMode = RenderMode::WORLD, glm::mat4 mvp = Transform::MAT4_IDENTITY, Expiration expiration = {0, 0});
    static void DrawRay(glm::vec3 start, glm::vec3 dir, Color color, int tickness, RenderMode renderMode = RenderMode::WORLD, glm::mat4 mvp = Transform::MAT4_IDENTITY, Expiration expiration = {0, 0});
    static void DrawPolygon(std::vector<glm::vec3> vertices_, Color color, int tickness, DrawMode mode = DrawMode::HOLLOW, RenderMode renderMode = RenderMode::WORLD, glm::mat4 mvp = Transform::MAT4_IDENTITY, Expiration expiration = {0, 0});
    static void DrawCircle(glm::vec3 position, float radius, Color color, int tickness, DrawMode mode = DrawMode::HOLLOW, RenderMode renderMode = RenderMode::WORLD, glm::mat4 mvp = Transform::MAT4_IDENTITY, Expiration expiration = {0, 0});
    static void DrawLine(glm::vec3 start, glm::vec3 end, Color color, int tickness, RenderMode renderMode = RenderMode::WORLD, glm::mat4 mvp = Transform::MAT4_IDENTITY, Expiration expiration = {0, 0});
    static void DrawRect(glm::vec3 position, Rect rect, Color color, int tickness, DrawMode mode = DrawMode::HOLLOW, RenderMode renderMode = RenderMode::WORLD, glm::mat4 mvp = Transform::MAT4_IDENTITY, Expiration expiration = {0, 0});
    static void DrawVector(glm::vec3 start, glm::vec3 end, Color color, int tickness, bool normalized = false, RenderMode renderMode = RenderMode::WORLD, glm::mat4 mvp = Transform::MAT4_IDENTITY, Expiration expiration = {0, 0});

    static void Clear();

private:
    static Shader* shader;
    static bool ready;
    static VertexArray* VAO;

    static bool Init();

    static std::map<int, float> expirations;
    static bool isExpired(Expiration expiration);
};
}  // namespace se
