#include "sepch.h"

#include "PhongShader.h"
#include "Shushao/Resources.h"
#include "Shushao/SceneManager.h"

namespace se {

	PhongShader::PhongShader() {
		name = "Phong Shader";

		VertexShaderCode = Resources::GetEmbeddedText(PHONGSHADER_VERT, LIB_SHUSHAO);
		FragmentShaderCode = Resources::GetEmbeddedText(PHONGSHADER_FRAG, LIB_SHUSHAO);
	}

	void PhongShader::Awake() {
		// material controlled uniforms
		AddUniform("Ambient reflection", "ambient_color", Uniform::Type::COLOR);
		AddUniform("Diffuse reflection", "diffuse_color", Uniform::Type::COLOR);
		AddUniform("Specular reflection", "specular_color", Uniform::Type::COLOR);
		AddUniform("Shininess", "shininess", Uniform::Type::FLOAT);

		AddUniform("Diffuse Map", "diffuse_map", Uniform::Type::TEXTURE);
		AddUniform("Specular Map", "specular_map", Uniform::Type::TEXTURE);
		AddUniform("Normal Map", "normal_map", Uniform::Type::TEXTURE);

		// shader controlled uniforms (locked)
		AddShaderUniform("view_position", Uniform::Type::VECTOR);
		AddShaderUniform("M", Uniform::Type::MATRIX);
		AddShaderUniform("point_lights_number", Uniform::Type::INTEGER);
		AddShaderUniform("spot_lights_number", Uniform::Type::INTEGER);
		// AddShaderUniform("directional_light", Uniform::Type::LIGHT);
		AddShaderUniform("enabled_diffuse_map", Uniform::Type::INTEGER);
		AddShaderUniform("enabled_normal_map", Uniform::Type::INTEGER);
		AddShaderUniform("enabled_specular_map", Uniform::Type::INTEGER);
	}

	void PhongShader::Update() {
		int pointLightCount = 0;
		int spotLightCount = 0;

		for (Light* light : SceneManager::activeScene->ActiveLights) {
			UniformLight ul = light->GetUniform();
			switch (light->type) {
			case Light::Type::DIRECTIONAL:
				SetLight("directional_light", ul);
				break;

			case Light::Type::POINT:
				SetLight("point_lights[" + ts(pointLightCount++) + "]", ul);
				break;

			case Light::Type::SPOT:
				SetLight("spot_lights[" + ts(spotLightCount++) + "]", ul);
				break;

			case Light::Type::DISC:
			case Light::Type::RECTANGLE:
				break;
			}
		}

		SetInteger("point_lights_number", pointLightCount);
		SetInteger("spot_lights_number", spotLightCount);

		glm::vec3 viewPosition = SceneManager::activeScene->activeCamera->transform->localPosition;
		SetVector("view_position", { viewPosition.x, viewPosition.y, viewPosition.z });
	}
}  // namespace se
