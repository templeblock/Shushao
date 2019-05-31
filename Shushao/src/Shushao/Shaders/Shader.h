#pragma once

#include "Shushao/Core.h"
#include "Shushao/Color.h"
#include "Shushao/Light.h"
#include "Shushao/Object.h"

namespace se {

enum ShaderLocation {
    LOCATION_POSITION = 1,
    LOCATION_TEXCOORD = 2,
    LOCATION_COLOR = 3,
    LOCATION_NORMAL = 4,
    LOCATION_MVP = 5,
    LOCATION_RENDER_COLOR = 6,
    LOCATION_SIZE = 7
};

class Uniform {
public:
    enum class Type {
        FLOAT,
        INTEGER,
        MATRIX,
        VECTOR,
        TEXTURE,
        COLOR,
        LIGHT
    };

    Uniform(std::string name_, std::string var_, Uniform::Type type_, unsigned int location_, bool locked_ = false) : name(name_), var(var_), type(type_), location(location_), locked(locked_) {}

    std::string name;
    std::string var;
    Uniform::Type type;
    unsigned int location = 0;
    bool locked = false;

    // values
    unsigned int texture;

    void SetFloat(float&);
    void SetInteger(int&);
    void SetTextureIndex(int&);
    void SetMatrix(float*);
    void SetVector(glm::vec2&);
    void SetVector(glm::vec3&);
    void SetVector(glm::vec4&);
    void SetColor(Color&);
    void SetLight(UniformLight&);
};

class Shader : public Object {
public:
    std::map<std::string, Uniform> uniforms;

    Shader();
    Shader(std::string);
    Shader(std::string, std::string);
    ~Shader();

    bool inUse = false;

    bool Init();
    void Use();
    void Leave();
    bool Load(std::string);
    void LoadFromString(std::string vsc, std::string fsc, std::string gsc = "");

    void awake();
    void update();
    void exit();

    unsigned int GetProgram();

    void AddUniform(std::string name_, std::string var_, Uniform::Type type_, unsigned int location_ = 0);
    void AddUniform(std::string var_, Uniform::Type type_, unsigned int location_ = 0);
    void AddShaderUniform(std::string var_, Uniform::Type type_, unsigned int location_ = 0);
    void AddShaderUniform(std::string name_, std::string var_, Uniform::Type type_, unsigned int location_ = 0);

    void SetFloat(std::string, float);
    void SetInteger(std::string, int);
    void SetTextureIndex(std::string, int);
    void SetMatrix(std::string, float*);
    void SetVector(std::string, glm::vec2);
    void SetVector(std::string, glm::vec3);
    void SetVector(std::string, glm::vec4);
    void SetColor(std::string, Color);
    void SetLight(std::string, UniformLight&);

    unsigned int GetTextureIndex(std::string);

    void SetMVP(float*);
    void SetMV(float*);
    void SetVP(float*);
    void SetP(float*);
    void SetV(float*);
    void SetM(float*);
    void SetRenderColor(Color);

    void Enable(std::string var);
    void Disable(std::string var);

protected:
    // per override
    virtual void Awake();
    virtual void Update();
    virtual void Exit();

    std::string VertexShaderCode;
    std::string FragmentShaderCode;
    std::string GeometryShaderCode;

private:
    unsigned int programID = 0;

    bool debug = true;

    unsigned int VertexShaderID;
    unsigned int FragmentShaderID;
    unsigned int GeometryShaderID;

    bool loadWithName(std::string, std::string);

    bool compile();
    bool link();

    bool shaderCompilationLog(const unsigned int&);
    bool programCompilationLog(const unsigned int&);
};

}  // namespace se
