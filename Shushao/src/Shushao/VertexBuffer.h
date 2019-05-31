#ifndef SE_VBO
#define SE_VBO

#include "Core.h"
#include "sepch.h"

#include "Object.h"

namespace se {

class VboConfiguration {
public:
    VboConfiguration(
        unsigned int target_ = 0x8892,
        unsigned int location_ = 0,
        int blocksize_ = 3,
        unsigned int type_ = 0x1406,
        unsigned char normalized_ = 0,
        int stride_ = 0,
        const void* pointer_ = (void*)0,
        unsigned int usage_ = 0x88E4) {
        target = target_;
        location = location_;
        blocksize = blocksize_;
        type = type_;
        normalized = normalized_;
        stride = stride_;
        pointer = pointer_;
        usage = usage_;
    }

    unsigned int target = 0x8892;
    unsigned int location = 0;
    int blocksize = 3;
    unsigned int type = 0x1406;
    unsigned char normalized = 0;
    int stride = 0;
    const void* pointer = (void*)0;
    unsigned int usage = 0x88E4;
};

extern VboConfiguration VBO_CONFIG_VERTEX;
extern VboConfiguration VBO_CONFIG_INDEX;
extern VboConfiguration VBO_CONFIG_NORMAL;
extern VboConfiguration VBO_CONFIG_COLOR;
extern VboConfiguration VBO_CONFIG_UV;
extern VboConfiguration VBO_CONFIG_FONT;

class VertexBuffer : public Object {
public:
    static const std::string VERTICES;
    static const std::string NORMALS;
    static const std::string INDEXES;
    static const std::string COLORS;
    static const std::string SIZES;
    static const std::string UV;

    VertexBuffer(VboConfiguration config_) : config(config_) {}
    ~VertexBuffer();

    unsigned int Id = 0;  // VertexBuffer ID GL
    signed long long int buffersize = 0;
    unsigned int size = 0;
    bool ready = false;

    VboConfiguration config;

    VertexBuffer* Init();
    VertexBuffer* Delete();
    VertexBuffer* Bind();
    VertexBuffer* Unbind();

    template <class T>
    VertexBuffer* Load(std::vector<T>& elements) {
        if (!ready) {
            return this;
        }
        Bind();
        signed long long int oldsize = buffersize;
        size = elements.size();
        buffersize = elements.size() * sizeof(T);
        if (oldsize == buffersize) {
            BufferSubData(config.target, 0, buffersize, &elements[0]);
        } else {
            BufferData(config.target, buffersize, &elements[0], config.usage);
        }
        Unbind();
        return this;
    }

    // per mascherare l'implementazione dei metodi glad/opengl
    void BufferSubData(unsigned int target, signed long long int offset, signed long long int size, const void* data);
    void BufferData(unsigned int target, signed long long int size, const void* data, int usage);
};

}  // namespace se

#endif
