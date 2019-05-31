#include "sepch.h"

#include <Box2D/Box2D.h>
#include <glad/glad.h>

#include "Shushao/Debug.h"
#include "Shushao/Design.h"
#include "Shushao/GLManager.h"
#include "Shushao/SceneManager.h"
#include "Shushao/shaders/BaseShader.h"
#include "PhysicsDebugDraw.h"

namespace se {

	bool PhysicsDebugDraw::Init() {
		if (!GLManager::ready || !Config::Physics::debug) return false;
		if (ready) return true;

		shader = new BaseShader();
		shader->awake();
		shader->Use();

		VAO = new VertexArray();
		VAO->AddBuffer(VertexBuffer::VERTICES, VBO_CONFIG_VERTEX);
		VAO->Init();

		shader->Leave();

		ready = true;
		return true;
	}

	void PhysicsDebugDraw::DrawPolygon(const b2Vec2* b2vertices, int32 vertexCount, const b2Color& color) {
		if (!Init()) return;

		std::vector<glm::vec3> vertices;

		for (int i = 0; i < vertexCount; i++) {
			vertices.push_back({ b2vertices[i].x, b2vertices[i].y, 0.0f });
		}

		shader->Use();
		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		glm::mat4 MVP = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();

		shader->SetMVP(&MVP[0][0]);
		shader->SetRenderColor({ 0.0f, 1.0f, 0.0f, alpha });

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		VAO->Leave();
		shader->Leave();
	}

	void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* b2vertices, int32 vertexCount, const b2Color& color) {
		if (!Init()) return;

		std::vector<glm::vec3> vertices;

		for (int i = 0; i < vertexCount; i++) {
			vertices.push_back({ b2vertices[i].x, b2vertices[i].y, 0.0f });
		}

		shader->Use();
		shader->update();

		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		glm::mat4 MVP = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();
		shader->SetMVP(&MVP[0][0]);

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		shader->SetRenderColor({ color.r, color.g, color.b, color.a * alpha });
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
		shader->SetRenderColor({ 0.0f, 1.0f, 0.0f, alpha });
		glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		VAO->Leave();
		shader->Leave();
	}

	void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
		if (!Init()) return;

		std::vector<glm::vec3> vertices;
		int NUM_DIVISIONS = 31;

		for (int i = 0; i < NUM_DIVISIONS + 1; i++) {
			vertices.push_back({ center.x + radius * cos((float)i / NUM_DIVISIONS * M_PI * 2), center.y + radius * sin((float)i / NUM_DIVISIONS * M_PI * 2), 0.0f });
		}

		shader->Use();

		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);
		shader->update();

		glm::mat4 MVP = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();
		shader->SetMVP(&MVP[0][0]);

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glLineWidth(2);
		shader->SetRenderColor({ color.r, color.g, color.b, alpha });
		glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
		glLineWidth(1);
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		VAO->Leave();
		shader->Leave();
	}

	void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
		if (!Init()) return;

		std::vector<glm::vec3> vertices;
		int NUM_DIVISIONS = 31;

		for (int i = 0; i < NUM_DIVISIONS + 1; i++) {
			if (i % 3 == 0) vertices.push_back({ center.x, center.y, 0.0f });
			vertices.push_back({ center.x + radius * cos((float)i / NUM_DIVISIONS * M_PI * 2), center.y + radius * sin((float)i / NUM_DIVISIONS * M_PI * 2), 0.0f });
		}

		shader->Use();
		shader->update();

		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		glm::mat4 MVP = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();
		shader->SetMVP(&MVP[0][0]);

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glLineWidth(2);
		shader->SetRenderColor({ color.r, color.g, color.b, alpha });
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
		glLineWidth(1);
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		VAO->Leave();
		shader->Leave();
	}

	void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
		if (!Init()) return;

		std::vector<glm::vec3> vertices;
		vertices = {
			{p1.x, p1.y, 0.0f},
			{p2.x, p2.y, 0.0f},
		};

		shader->Use();
		shader->update();

		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		glm::mat4 MVP = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();
		shader->SetMVP(&MVP[0][0]);

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		shader->SetRenderColor({ color.r, color.g, color.b, color.a * alpha });
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

		shader->SetRenderColor({ color.r, color.g, color.b, alpha });
		glDrawArrays(GL_LINE, 0, vertices.size());
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		VAO->Leave();
		shader->Leave();
	}

	void PhysicsDebugDraw::DrawTransform(const b2Transform& xf) {
		if (!Init()) return;

		std::vector<glm::vec3> vertices;
		vertices = {
			{0.2f, 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f},
			{0.0f, 0.2f, 0.0f} };

		glm::quat rotation = glm::quat({ 0.0f, 0.0f, xf.q.GetAngle() });

		for (glm::vec3& v : vertices) {
			v = (rotation * v) + glm::vec3({ xf.p.x, xf.p.y, 0.0f });
		}

		shader->Use();
		shader->update();

		VAO->Use();
		VAO->Load<glm::vec3>(VertexBuffer::VERTICES, vertices);

		glm::mat4 MVP = SceneManager::activeScene->activeCamera->Projection * SceneManager::activeScene->activeCamera->getViewMatrix() * glm::mat4();
		shader->SetMVP(&MVP[0][0]);

		VAO->GetBuffer(VertexBuffer::VERTICES)->Bind();
		glEnablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		glLineWidth(2);
		shader->SetRenderColor({ 1.0f, 0.0f, 0.0f, alpha });
		glDrawArrays(GL_LINE_STRIP, 0, 2);
		shader->SetRenderColor({ 0.0f, 1.0f, 0.0f, alpha });
		glDrawArrays(GL_LINE_STRIP, 1, 2);
		glLineWidth(1);
		glDisablei(GL_BLEND, VAO->GetBuffer(VertexBuffer::VERTICES)->Id);
		VAO->GetBuffer(VertexBuffer::VERTICES)->Unbind();

		VAO->Leave();
		shader->Leave();
	}

}  // namespace se
