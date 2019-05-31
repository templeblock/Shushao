#ifndef SE_VAO
#define SE_VAO


#include "Core.h"
#include "sepch.h"

#include "Color.h"
#include "Debug.h"
#include "Object.h"
#include "Shaders/Shader.h"
#include "VertexBuffer.h"

namespace se {

class VertexArray : public Object {
public:
    VertexArray();
    ~VertexArray();

    unsigned int Id = 0;  // Vao ID GL
    bool inUse = false;
    bool ready = false;

    std::map<std::string, VertexBuffer*> buffers;

    VertexArray* Init();
    VertexArray* Use();
    VertexArray* Leave();
    VertexArray* EnablePointers();
    VertexArray* DisablePointers();

    VertexBuffer* AddBuffer(std::string name_, VboConfiguration config_);
    VertexBuffer* AddBuffer(VertexBuffer* vbo);
    VertexBuffer* GetBuffer(std::string name);

    template <class T>
    VertexArray* Load(std::string name_, std::vector<T>& elements) {
        if (!inUse) {
            Debug::Log(ERROR) << "VAO is not in use" << std::endl;
            return this;
        }

        auto it = buffers.find(name_);
        if (it == buffers.end()) {
            Debug::Log(ERROR) << "VBO " << name_ << " not found" << std::endl;
            return this;
        }

        if (!buffers[name_]->ready) {
            buffers[name_]->Init();
        }

        buffers[name_]->Load<T>(elements);

        return this;
    }
};

}  // namespace se

#endif