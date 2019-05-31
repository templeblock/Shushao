#include "sepch.h"

#include <glad/glad.h>

#include "Core.h"
#include "Design.h"
#include "GLManager.h"
#include "SceneManager.h"
#include "Shaders/BaseShader.h"

namespace se {

	bool Design::Init() {
		if (ready) return true;
		if (!GLManager::ready) return false;

		shader = new BaseShader();
		shader->awake();
		shader->Use();

		VAO = new VertexArray();
		VAO->AddBuffer(VertexBuffer::VERTICES, VBO_CONFIG_VERTEX);
		VAO->Init();

		return ready = true;
	}

	void Design::Clear() {
		expirations.clear();
		if (shader != nullptr) delete (shader);
	}

	//{ #region draw methods

	void Design::DrawPoint(glm::vec3 position, Color color, int tickness, RenderMode renderMode, glm::mat4 mvp, Expiration expiration) {
		if (!Init()) return;
		if (isExpired(expiration)) return;

		std::vector<glm::vec3> vertices = { position };

		shader->Use();
		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		if (mvp == Transform::MAT4_IDENTITY || renderMode == RenderMode::SCREEN)
			mvp = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();

		if (renderMode == RenderMode::SCREEN) {
			shader->Enable("viewport");
			shader->SetVector("viewport", GLManager::VIEWPORT);
		}

		shader->SetMVP(&mvp[0][0]);
		shader->SetRenderColor(color);

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();

		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glPointSize(tickness);
		glDrawArrays(GL_POINTS, 0, 1);
		glPointSize(1);
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);

		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		if (renderMode == RenderMode::SCREEN) {
			shader->Disable("viewport");
		}

		VAO->Leave();
		shader->Leave();
	}

	void Design::DrawLine(glm::vec3 start, glm::vec3 end, Color color, int tickness, RenderMode renderMode, glm::mat4 mvp, Expiration expiration) {
		if (!Init()) return;
		if (isExpired(expiration)) return;

		std::vector<glm::vec3> vertices = { start, end };

		shader->Use();
		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		if (mvp == Transform::MAT4_IDENTITY || renderMode == RenderMode::SCREEN)
			mvp = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();

		if (renderMode == RenderMode::SCREEN) {
			shader->Enable("viewport");
			shader->SetVector("viewport", GLManager::VIEWPORT);
		}

		shader->SetMVP(&mvp[0][0]);
		shader->SetRenderColor(color);

		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glLineWidth(tickness);
		glDrawArrays(GL_LINES, 0, vertices.size());
		glLineWidth(1);
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);

		if (renderMode == RenderMode::SCREEN) {
			shader->Disable("viewport");
		}

		VAO->Leave();
		shader->Leave();
	}

	void Design::DrawVector(glm::vec3 start, glm::vec3 end, Color color, int tickness, bool normalized, RenderMode renderMode, glm::mat4 mvp, Expiration expiration) {
		if (!Init()) return;
		if (isExpired(expiration)) return;

		GLfloat bxs = 0.02f;
		if (normalized) end = normalize(end);
		std::vector<glm::vec3> vertices = {
			glm::vec3(-bxs, -bxs, 0.0f) + start,
			glm::vec3(-bxs, bxs, 0.0f) + start,
			glm::vec3(bxs, bxs, 0.0f) + start,
			glm::vec3(bxs, -bxs, 0.0f) + start,
			glm::vec3(-bxs, -bxs, 0.0f) + start,
			start,
			start + end,
			start };

		shader->Use();
		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		if (mvp == Transform::MAT4_IDENTITY || renderMode == RenderMode::SCREEN)
			mvp = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();

		shader->SetMVP(&mvp[0][0]);
		shader->SetRenderColor(color);

		if (renderMode == RenderMode::SCREEN) {
			shader->Enable("viewport");
			shader->SetVector("viewport", GLManager::VIEWPORT);
		}

		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glLineWidth(tickness);
		glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
		glLineWidth(1);
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);

		if (renderMode == RenderMode::SCREEN) {
			shader->Disable("viewport");
		}

		VAO->Leave();
		shader->Leave();
	}

	void Design::DrawRay(glm::vec3 start, glm::vec3 dir, Color color, int tickness, RenderMode renderMode, glm::mat4 mvp, Expiration expiration) {
		if (!Init()) return;
		if (isExpired(expiration)) return;

		std::vector<glm::vec3> vertices = { start, start + dir };

		shader->Use();
		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		if (mvp == Transform::MAT4_IDENTITY || renderMode == RenderMode::SCREEN)
			mvp = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();

		if (renderMode == RenderMode::SCREEN) {
			shader->Enable("viewport");
			shader->SetVector("viewport", GLManager::VIEWPORT);
		}

		shader->SetMVP(&mvp[0][0]);
		shader->SetRenderColor(color);

		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glLineWidth(tickness);
		glDrawArrays(GL_LINES, 0, vertices.size());
		glLineWidth(1);
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);

		if (renderMode == RenderMode::SCREEN) {
			shader->Disable("viewport");
		}

		VAO->Leave();
		shader->Leave();
	}

	void Design::DrawPolygon(std::vector<glm::vec3> vertices, Color color, int tickness, DrawMode mode, RenderMode renderMode, glm::mat4 mvp, Expiration expiration) {
		if (!Init()) return;
		if (isExpired(expiration)) return;

		shader->Use();
		VAO->Use();

		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		if (mvp == Transform::MAT4_IDENTITY || renderMode == RenderMode::SCREEN) {
			mvp = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();
		}

		shader->SetMVP(&mvp[0][0]);

		if (renderMode == RenderMode::SCREEN) {
			shader->Enable("viewport");
			shader->SetVector("viewport", GLManager::VIEWPORT);
		}

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glLineWidth(tickness);

		if (mode == DrawMode::BORDERED)
			shader->SetRenderColor({ color.r, color.g, color.b, color.a / 2 });
		else
			shader->SetRenderColor(color);

		glDrawArrays((mode != DrawMode::HOLLOW ? GL_TRIANGLE_FAN : GL_LINE_LOOP), 0, vertices.size());

		if (mode == DrawMode::BORDERED) {
			shader->SetRenderColor(color);
			glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
		}

		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glLineWidth(1);
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		if (renderMode == RenderMode::SCREEN) {
			shader->Disable("viewport");
		}

		VAO->Leave();
		shader->Leave();
	}

	void Design::DrawCircle(glm::vec3 position, float radius, Color color, int tickness, DrawMode mode, RenderMode renderMode, glm::mat4 mvp, Expiration expiration) {
		if (!Init()) return;
		if (isExpired(expiration)) return;

		std::vector<glm::vec3> vertices;
		int NUM_DIVISIONS = 31;

		for (int i = 0; i < NUM_DIVISIONS + 1; i++) {
			if (mode == DrawMode::FULL)
				if (i % 3 == 0) vertices.push_back({ position.x, position.y, 0.0f });
			vertices.push_back({ position.x + radius * cos((float)i / NUM_DIVISIONS * M_PI * 2), position.y + radius * sin((float)i / NUM_DIVISIONS * M_PI * 2), 0.0f });
		}

		shader->Use();
		VAO->Use();

		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		if (mvp == Transform::MAT4_IDENTITY || renderMode == RenderMode::SCREEN) {
			mvp = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();
		}

		shader->SetMVP(&mvp[0][0]);
		shader->SetRenderColor(color);

		if (renderMode == RenderMode::SCREEN) {
			shader->Enable("viewport");
			shader->SetVector("viewport", GLManager::VIEWPORT);
		}

		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glLineWidth(tickness);
		glDrawArrays((mode == DrawMode::FULL ? GL_TRIANGLE_STRIP : GL_LINE_LOOP), 0, vertices.size());
		glLineWidth(1);
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);

		if (renderMode == RenderMode::SCREEN) {
			shader->Disable("viewport");
		}

		VAO->Leave();
		shader->Leave();
	}

	void Design::DrawRect(glm::vec3 position, Rect rect, Color color, int tickness, DrawMode mode, RenderMode renderMode, glm::mat4 mvp, Expiration expiration) {
		if (!Init()) return;

		if (isExpired(expiration)) return;

		rect.SetPosition(rect.position + glm::vec2(position));
		std::vector<glm::vec3> vertices = rect.GetVertices3D();

		DrawPolygon(vertices, color, tickness, mode, renderMode, mvp, expiration);
	}

	bool Design::isExpired(Expiration expiration) {
		bool expired = false;
		if (expiration.duration > 0.0f) {
			auto it = expirations.find(expiration.id);
			if (it == expirations.end()) {
				expired = false;
				expirations.insert({ expiration.id, Time::time + expiration.duration });
			}
			else {
				expired = it->second <= Time::time;
			}
		}
		return expired;
	}

	//}

	// init
	Shader* Design::shader = nullptr;
	bool Design::ready;
	VertexArray* Design::VAO;
	std::map<int, float> Design::expirations;
}  // namespace se
