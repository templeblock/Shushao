#include "sepch.h"

#include "StandardShader.h"
#include "Shushao/Resources.h"

namespace se {

	StandardShader::StandardShader() {
		name = "Standard Shader";

		VertexShaderCode = Resources::GetEmbeddedText(STANDARDSHADER_VERT, LIB_SHUSHAO);
		FragmentShaderCode = Resources::GetEmbeddedText(STANDARDSHADER_FRAG, LIB_SHUSHAO);
	}

	void StandardShader::Awake() {
		// custom uniforms
		AddUniform("Diffuse Map", "diffuse_map", Uniform::Type::TEXTURE);
		AddUniform("Render Color", "render_color", Uniform::Type::TEXTURE, ShaderLocation::LOCATION_RENDER_COLOR);

		AddShaderUniform("image_border", Uniform::Type::VECTOR);
		AddShaderUniform("sprite_border", Uniform::Type::VECTOR);
		AddShaderUniform("image_size", Uniform::Type::VECTOR);
		AddShaderUniform("texture_size", Uniform::Type::VECTOR);
		AddShaderUniform("enabled_sliced", Uniform::Type::INTEGER);
		AddShaderUniform("enabled_tiled", Uniform::Type::INTEGER);
		AddShaderUniform("sliced_fill", Uniform::Type::INTEGER);

		// valorizzazione
		SetTextureIndex("diffuse_map", 0);
	}
}  // namespace se
