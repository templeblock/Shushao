#pragma once

#include "Core.h"
#include "sepch.h"

#include "Object.h"
#include "Texture.h"
#include "VertexArray.h"

namespace se {

class Mesh : public Object {
public:
    Mesh();
    Mesh(std::string objfilename);
    ~Mesh();

    VertexArray* VAO = nullptr;

    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec2> uvData;
    std::vector<glm::vec3> normalsData;
    std::vector<unsigned short int> indexesData;

    bool ready = false;

    Mesh* Init();
    Mesh* Load(std::string objectFile);

    void PrintData();
};

}  // namespace se