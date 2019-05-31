#include "sepch.h"

#include "Entity.h"
#include "GLManager.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Canvas.h"

namespace se {

	Canvas::~Canvas() {}

	void Canvas::Copy(Canvas* other) {
		if (other == nullptr) return;
		Component::Copy(other);

		SetRenderMode(other->renderMode);
		SetCamera(other->camera);
	}

	void Canvas::setup() {
		entity->transform->isRectTransform = true;
		entity->canvas = this;
	}

	Canvas* Canvas::SetRenderMode(RenderMode value) {
		_renderMode = value;
		_lastRenderMode = value;
		processRenderMode();
		return this;
	}

	Canvas* Canvas::SetCamera(Camera* camera_) {
		camera = camera_;
		return this;
	}

	void Canvas::processRenderMode() {
		switch (renderMode) {
		case RenderMode::SCREEN:
			transform->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
			transform->SetPivot(PivotPosition::BOTTOMLEFT);
			transform->rectTransform->SetRectSize(GLManager::VIEWPORT);
			break;
		case RenderMode::CAMERA:
			transform->SetLocalPosition({ 0.0f, 0.0f, 1.0f });
			transform->SetPivot(PivotPosition::CENTER);
			transform->rectTransform->SetRectSize(camera->getViewportSize());
		case RenderMode::WORLD:
			break;
		}
	}

	void Canvas::Awake() {
		processRenderMode();
	}

	void Canvas::Update() {
		processRenderMode();
	}

	void Canvas::Render() {
	}

	void Canvas::OnDestroy() {
	}

}  // namespace se
