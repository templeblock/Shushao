#include "sepch.h"

#include "ParticlesShader.h"
#include "Shushao/Resources.h"

namespace se {

	ParticlesShader::ParticlesShader() {
		name = "Particles Shader";

		VertexShaderCode = Resources::GetEmbeddedText(PARTICLESSHADER_VERT, LIB_SHUSHAO);
		FragmentShaderCode = Resources::GetEmbeddedText(PARTICLESSHADER_FRAG, LIB_SHUSHAO);
		GeometryShaderCode = Resources::GetEmbeddedText(PARTICLESSHADER_GEOM, LIB_SHUSHAO);
	}

	void ParticlesShader::Awake() {
		// custom uniforms
		AddUniform("Diffuse Map", "diffuse_map", Uniform::Type::TEXTURE);
		AddUniform("Render Color", "render_color", Uniform::Type::TEXTURE, ShaderLocation::LOCATION_RENDER_COLOR);

		// shader controlled uniforms (locked)
		AddShaderUniform("M", Uniform::Type::MATRIX);
		AddShaderUniform("V", Uniform::Type::MATRIX);
		AddShaderUniform("P", Uniform::Type::MATRIX);
		AddShaderUniform("enabled_worldspace", Uniform::Type::INTEGER);

		// valorizzazione
		SetTextureIndex("diffuse_map", 0);
	}
}  // namespace se
