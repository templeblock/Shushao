
#include "sepch.h"

#include <glad/glad.h>

#include "Debug.h"
#include "GLManager.h"
#include "Mesh.h"
#include "Shaders/phongShader.h"
#include "Transform.h"
#include "MeshRenderer.h"

namespace se {

	void MeshRenderer::setup() {
		material = new Material();
		material->SetShader(new PhongShader());
	}

	MeshRenderer::~MeshRenderer() {
		if (material != nullptr) delete (material);
	}

	bool MeshRenderer::isReady() {
		return (
			mesh != nullptr &&
			material != nullptr &&
			material->shader != nullptr &&
			transform != nullptr);
	}

	void MeshRenderer::Awake() {
		if (mesh == nullptr) {
			Debug::Log(ERROR) << "Mesh undefined" << std::endl;
			return;
		}

		if (mesh->VAO == nullptr) {
			Debug::Log(ERROR) << "Mesh VAO undefined" << std::endl;
			return;
		}
	}

	void MeshRenderer::Update() {
		if (!isReady()) return;
	}

	void MeshRenderer::Render() {
		if (!isReady()) return;

		mesh->VAO->Use();
		material->shader->Use();
		material->shader->SetMVP(transform->uMVP());
		material->shader->SetM(transform->uM());
		material->update();

		mesh->VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glDrawArrays(GL_TRIANGLES, 0, mesh->VAO->GetBuffer(VertexBuffer::VERTICES)->size);
		mesh->VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		/* mesh->VAO->GetBuffer(VertexBuffer::INDEXES)->Bind();
			glDrawElements(GL_TRIANGLES, mesh->VAO->GetBuffer(VertexBuffer::INDEXES)->size, GL_UNSIGNED_SHORT, 0);
			mesh->VAO->GetBuffer(VertexBuffer::INDEXES)->Unbind();*/

		material->shader->Leave();
		mesh->VAO->Leave();
	}

	void MeshRenderer::OnDestroy() {
		if (!isReady()) return;

		mesh->VAO->Leave();
		material->shader->Leave();
		material->shader->exit();
		delete (material);
		material = nullptr;
	}
}  // namespace se
