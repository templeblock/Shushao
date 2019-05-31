#include "sepch.h"

#include <glad/glad.h>

#include "Debug.h"
#include "Material.h"

namespace se {

	//{ #region parameter

	GLfloat Parameter::GetFloat() {
		return value.f;
	}

	GLint Parameter::GetInteger() {
		return value.i;
	}

	Texture* Parameter::GetTexture() {
		return value.tex;
	}

	glm::vec4 Parameter::GetVector() {
		return value.vec;
	}

	glm::mat4 Parameter::GetMatrix() {
		return value.mat;
	}

	Color Parameter::GetColor() {
		return value.col;
	}

	void Parameter::SetFloat(GLfloat value_) {
		value.f = value_;
	}

	void Parameter::SetInteger(GLint value_) {
		value.i = value_;
	}

	void Parameter::SetTexture(Texture* value_) {
		value.tex = value_;
	}

	void Parameter::SetVector(glm::vec4 value_) {
		value.vec = value_;
	}

	void Parameter::SetMatrix(glm::mat4 value_) {
		value.mat = value_;
	}

	void Parameter::SetColor(Color value_) {
		value.col = value_;
	}

	//}

	Material::Material() {}

	Material::~Material() {
		parameters.clear();
	}

	void Material::SetShader(Shader* shader_) {
		shader = shader_;
		init();
	}

	void Material::SetMainTexture(Texture* texture_) {
		mainTexture = texture_;
		SetTexture("diffuse_map", mainTexture);
	}

	void Material::AddParameter(std::string name_, std::string var_, Uniform* uniform_) {
		parameters.insert(std::pair<std::string, Parameter>(var_, { name_, var_, uniform_ }));
	}

	void Material::init() {
		if (shader == nullptr) {
			Debug::Log(ERROR) << "Shader not loaded" << std::endl;
		}

		parameters.clear();

		shader->awake();

		for (auto& it : shader->uniforms) {
			if (!it.second.locked) {
				AddParameter(it.second.name, it.first, &it.second);
			}
		}

		Init();
	}

	void Material::update() {
		if (!shader->inUse) shader->Use();
		shader->update();

		GLenum textureUnit = GL_TEXTURE0;

		for (const auto& it : parameters) {
			Parameter parameter = it.second;

			if (parameter.uniform->locked) {
				continue;
			}

			switch (parameter.uniform->type) {
			case Uniform::Type::INTEGER:
				parameter.uniform->SetInteger(parameter.value.i);
				break;
			case Uniform::Type::FLOAT:
				parameter.uniform->SetFloat(parameter.value.f);
				break;
			case Uniform::Type::MATRIX:
				parameter.uniform->SetMatrix(&parameter.value.mat[0][0]);
				break;
			case Uniform::Type::VECTOR:
				parameter.uniform->SetVector(parameter.value.vec);
				break;
			case Uniform::Type::COLOR:
				parameter.uniform->SetColor(parameter.value.col);
				break;
			case Uniform::Type::TEXTURE:
				if (parameter.value.tex != nullptr) {
					shader->Enable(parameter.var);
					GLint textureIndex = textureUnit - GL_TEXTURE0;
					parameter.uniform->SetTextureIndex(textureIndex);
					glActiveTexture(textureUnit);
					glBindTexture(GL_TEXTURE_2D, parameter.value.tex->TextureID);
					textureUnit++;
				}
			case Uniform::Type::LIGHT:
				break;
			}
		}

		Update();
	}

	//{ #region getters/setters values

	GLfloat Material::GetFloat(std::string var_) {
		const auto& it = parameters.find(var_);
		if (it != parameters.end())
			return it->second.value.f;
		return 0.0f;
	}

	GLint Material::GetInteger(std::string var_) {
		const auto& it = parameters.find(var_);
		if (it != parameters.end())
			return it->second.value.i;
		return 0;
	}

	Texture* Material::GetTexture(std::string var_) {
		const auto& it = parameters.find(var_);
		if (it != parameters.end())
			return it->second.value.tex;
		return nullptr;
	}

	glm::vec4 Material::GetVector(std::string var_) {
		const auto& it = parameters.find(var_);
		if (it != parameters.end())
			return it->second.value.vec;
		return { 0, 0, 0, 0 };
	}

	glm::mat4 Material::GetMatrix(std::string var_) {
		const auto& it = parameters.find(var_);
		if (it != parameters.end())
			return it->second.value.mat;
		glm::mat4 m;
		return m;
	}

	Color Material::GetColor(std::string var_) {
		const auto& it = parameters.find(var_);
		if (it != parameters.end())
			return it->second.value.col;
		return color::clear;
	}

	void Material::SetFloat(std::string var_, GLfloat value_) {
		auto it = parameters.find(var_);
		if (it != parameters.end())
			it->second.value.f = value_;
	}

	void Material::SetInteger(std::string var_, GLint value_) {
		auto it = parameters.find(var_);
		if (it != parameters.end())
			it->second.value.i = value_;
	}

	void Material::SetTexture(std::string var_, Texture* value_) {
		auto it = parameters.find(var_);
		if (it != parameters.end())
			it->second.value.tex = value_;
	}

	void Material::SetVector(std::string var_, glm::vec4 value_) {
		auto it = parameters.find(var_);
		if (it != parameters.end())
			it->second.value.vec = value_;
	}

	void Material::SetMatrix(std::string var_, glm::mat4 value_) {
		auto it = parameters.find(var_);
		if (it != parameters.end())
			it->second.value.mat = value_;
	}

	void Material::SetColor(std::string var_, Color value_) {
		auto it = parameters.find(var_);
		if (it != parameters.end())
			it->second.value.col = value_;
	}

	//}

}  // namespace se
