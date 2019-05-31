#include "sepch.h"

#include <glad/glad.h>

#include "Shushao/Core.h"
#include "Color.h"
#include "GeometryRenderer.h"
#include "GLManager.h"
#include "Shaders/WireframeShader.h"
#include "Transform.h"

namespace se {

	void GeometryRenderer::setup() {
		shader = new WireframeShader();
	}

	GeometryRenderer::~GeometryRenderer() {
		if (VAO != nullptr) {
			delete (VAO);
			VAO = nullptr;
		}
	}

	void GeometryRenderer::AddLine(const glm::vec3 start_point, const glm::vec3 end_point, Color col) {
		vertices.push_back(start_point);
		vertices.push_back(end_point);
		colors.push_back(col.rgba());
		colors.push_back(col.rgba());
	}

	void GeometryRenderer::AddPolyline(const std::vector<glm::vec3> points, Color col) {
		bool isFirst = true;
		glm::vec3 lastpoint;
		for (glm::vec3 point : points) {
			vertices.push_back(point);
			colors.push_back(col.rgba());
			if (!isFirst) {
				vertices.push_back(point);
				colors.push_back(col.rgba());
			}
			isFirst = false;
			lastpoint = point;
		}
	}

	void GeometryRenderer::AddCircle(glm::vec3 position, float radius, Color color, DrawMode mode) {
		// TODO: non funziona molto

		int NUM_DIVISIONS = 120;

		for (int i = 0; i < NUM_DIVISIONS + 1; i++) {
			if (mode == DrawMode::FULL)
				if (i % 3 == 0) {
					vertices.push_back({ position.x, position.y, 0.0f });
					colors.push_back(color.rgba());
				}
			vertices.push_back({ position.x + radius * cos((float)i / NUM_DIVISIONS * M_PI * 2), position.y + radius * sin((float)i / NUM_DIVISIONS * M_PI * 2), 0.0f });
			colors.push_back(color.rgba());
		}
	}

	void GeometryRenderer::Awake() {
		shader->awake();
		shader->Use();

		VAO = new VertexArray();
		VAO->AddBuffer(VertexBuffer::VERTICES, VBO_CONFIG_VERTEX);
		VAO->AddBuffer("colors", VBO_CONFIG_COLOR);
		VAO->Init();

		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);
		VAO->Load<glm::vec4>("colors", colors);
		VAO->Leave();

		shader->Leave();
	}

	void GeometryRenderer::Update() {}

	void GeometryRenderer::Render() {
		shader->Use();
		VAO->Use();

		shader->SetMVP(transform->uMVP());
		shader->update();

		if (renderMode == RenderMode::SCREEN) {
			shader->SetM(transform->uM());
			shader->Enable("viewport");
			shader->SetVector("viewport", GLManager::VIEWPORT);
		}

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glDrawArrays(GL_LINES, 0, vertices.size());
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		if (renderMode == RenderMode::SCREEN) {
			shader->Disable("viewport");
		}

		VAO->Leave();
		shader->Use();
	}

	void GeometryRenderer::OnDestroy() {
		shader->Leave();
		VAO->Leave();
	}
}  // namespace se
