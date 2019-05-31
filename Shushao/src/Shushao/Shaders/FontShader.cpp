#include "sepch.h"

#include "FontShader.h"
#include "Shushao/Resources.h"

namespace se {

FontShader::FontShader() {
    name = "Font Shader";

    VertexShaderCode = Resources::GetEmbeddedText(FONTSHADER_VERT, LIB_SHUSHAO);
    FragmentShaderCode = Resources::GetEmbeddedText(FONTSHADER_FRAG, LIB_SHUSHAO);
}

void FontShader::Awake() {
    AddUniform("Diffuse Map", "diffuse_map", Uniform::Type::TEXTURE);

    AddShaderUniform("M", Uniform::Type::MATRIX);
    AddShaderUniform("viewport", Uniform::Type::VECTOR);
    AddShaderUniform("enabled_viewport", Uniform::Type::INTEGER);

    SetTextureIndex("diffuse_map", 0);
}
}  // namespace se
