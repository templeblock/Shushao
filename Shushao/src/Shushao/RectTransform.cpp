#include "sepch.h"

#include "canvas.h"
#include "Debug.h"
#include "Design.h"
#include "Entity.h"
#include "Transform.h"

namespace se {

	RectTransform::RectTransform(Transform* transform_) {
		transform = transform_;
		init();
		update();
		_rect.YUP = true;
		_rect.name = transform->entity->name;
	}

	glm::mat4 RectTransform::GetLocalToParentMatrix() {
		glm::mat4 matrix;
		if (hasSingleAnchorPoint()) {
			glm::vec2 parentAnchorCoord = parentRectTransform->rectCoordToLocal(anchorMin * parentRectTransform->rect.size);
			matrix = glm::translate(glm::mat4(), glm::vec3(parentAnchorCoord, 0.0f)) * glm::translate(glm::mat4(), transform->localPosition) * glm::toMat4(transform->localRotation) * glm::scale(glm::mat4(), transform->localScale);
		}
		else {
			if (rectInvalid) SetSizeWithCurrentAnchors();
			matrix = glm::translate(glm::mat4(), glm::vec3(anchoredPosition, 0.0f)) * glm::toMat4(transform->localRotation) * glm::scale(glm::mat4(), transform->localScale);
		}

		return matrix;
	}

	void RectTransform::update() {
		if (isRectTransformChild && !hasSingleAnchorPoint()) {
			// if (rectInvalid) SetSizeWithCurrentAnchors();
		}
		else {
			_rect.SetPosition(glm::vec2(0, rect.height) - localPivot());
		}
	}

	glm::vec2 RectTransform::rectCoordToLocal(glm::vec2 pcoord) {
		return pcoord - localPivot();
	}

	glm::vec2 RectTransform::localPivot(bool flipY) {
		if (flipY) {
			return rect.size * pivot * glm::vec2(1, -1);
		}
		return rect.size * pivot;
	}

	Rect RectTransform::GetAnchorsParentRect() {
		Rect ar;
		ar.YUP = true;
		ar.Set(
			anchorMin.x * parentRectTransform->rect.width,
			anchorMax.y * parentRectTransform->rect.height,
			anchorMax.x * parentRectTransform->rect.width - anchorMin.x * parentRectTransform->rect.width,
			anchorMax.y * parentRectTransform->rect.height - anchorMin.y * parentRectTransform->rect.height);

		return ar;
	}

	void RectTransform::SetSizeWithCurrentAnchors() {
		if (!isRectTransformChild) return;
		Rect apr = GetAnchorsParentRect();
		glm::vec2 piv = pivot * glm::vec2(deltas.width, deltas.height);

		Rect tempRect;
		tempRect.YUP = true;

		switch (registerMode) {
		case RegisterMode::XYWH:  // OK
			deltas.left = deltas.X - piv.x;

			tempRect.Set(
				apr.xMin + deltas.left,
				apr.yMax + deltas.Y + pivot.y,
				deltas.width,
				deltas.height);

			transform->SetLocalPosition({ deltas.X, deltas.Y, 0.0f });
			SetRectSize({ deltas.width, deltas.height });

			break;

		case RegisterMode::LRYH:  // OK
			deltas.width = parentRectTransform->rect.width - deltas.left - deltas.right;

			tempRect.Set(
				apr.xMin + deltas.left,
				apr.yMax + deltas.Y + pivot.y,
				deltas.width,
				deltas.height);

			break;

		case RegisterMode::TBXW:  // OK
			deltas.left = deltas.X - piv.x;
			deltas.right = parentRectTransform->rect.width - deltas.width - deltas.left;
			deltas.height = parentRectTransform->rect.height - deltas.top - deltas.bottom;

			tempRect.Set(
				apr.xMin + deltas.left,
				apr.yMax - deltas.top,
				deltas.width,
				deltas.height);

			break;

		case RegisterMode::LRTH:  // OK
			deltas.width = parentRectTransform->rect.width - deltas.left - deltas.right;

			tempRect.Set(
				apr.xMin + deltas.left,
				apr.yMax - deltas.top,
				deltas.width,
				deltas.height);

			break;

		case RegisterMode::LRTB:  // OK
			deltas.width = parentRectTransform->rect.width - deltas.left - deltas.right;
			deltas.height = parentRectTransform->rect.height - deltas.top - deltas.bottom;

			tempRect.Set(
				apr.xMin + deltas.left,
				apr.yMax - deltas.top,
				deltas.width,
				deltas.height);

			break;
		}

		glm::vec2 localPosition = tempRect.position - parentRectTransform->localPivot();

		_rect.SetSize(tempRect.size);
		_rect.SetPosition(glm::vec2(0, 0) - localPivot(true));
		_anchoredPosition = localPosition + localPivot(true);

		/* Debug::Log << "AnchorsParentRect" << std::endl;
			Debug::Log << apr.ToString() << std::endl;
			Debug::Log << "Rect" << std::endl;
			Debug::Log << tempRect.ToString() << std::endl;
			Debug::Log << "Anchored Position: " <<_anchoredPosition.x << ", " << _anchoredPosition.y << std::endl;
			Debug::Log(WARNING) << deltas.toString() << std::endl; */

		rectInvalid = false;
	}

	void RectTransform::RegisterPositionXYWH(float X_, float Y_, float width_, float height_) {  // OK

		deltas.X = X_;
		deltas.Y = Y_;
		deltas.width = width_;
		deltas.height = height_;

		registerMode = RegisterMode::XYWH;
		SetSizeWithCurrentAnchors();
	}

	void RectTransform::RegisterPositionLRYH(float left_, float right_, float Y_, float height_) {  // OK
		deltas.left = left_;
		deltas.right = right_;
		deltas.Y = Y_;
		deltas.height = height_;

		registerMode = RegisterMode::LRYH;
		SetSizeWithCurrentAnchors();
	}

	void RectTransform::RegisterPositionTBXW(float top_, float bottom_, float X_, float width_) {
		deltas.top = top_;
		deltas.bottom = bottom_;
		deltas.X = X_;
		deltas.width = width_;

		registerMode = RegisterMode::TBXW;
		SetSizeWithCurrentAnchors();
	}

	void RectTransform::RegisterPositionLRTH(float left_, float right_, float top_, float height_) {
		deltas.left = left_;
		deltas.right = right_;
		deltas.top = top_;
		deltas.height = height_;

		registerMode = RegisterMode::LRTH;
		SetSizeWithCurrentAnchors();
	}

	void RectTransform::RegisterPositionLRTB(float left_, float right_, float top_, float bottom_) {
		deltas.left = left_;
		deltas.right = right_;
		deltas.top = top_;
		deltas.bottom = bottom_;

		registerMode = RegisterMode::LRTB;
		SetSizeWithCurrentAnchors();
	}

	void RectTransform::SetPivot(glm::vec2 value) {
		if (value != _pivot) {
			_pivot = value;
			// transform->Invalidate();
			update();
		}
	}

	void RectTransform::SetAnchor(glm::vec2 value) {
		_anchorMax = value;
		_anchorMin = value;
		update();
	}

	void RectTransform::SetAnchor(AnchorPoints points) {
		_anchorMin = points.min;
		_anchorMax = points.max;
		update();
	}

	void RectTransform::SetAnchorMax(glm::vec2 value) {
		_anchorMax = value;
		update();
	}

	void RectTransform::SetAnchorMin(glm::vec2 value) {
		_anchorMin = value;
		update();
	}

	void RectTransform::SetRect(Rect value) {
		if (value != _rect) {
			InvalidateChildren();
			_rect = value;
			transform->Invalidate();
			update();
		}
	}

	void RectTransform::SetRectSize(glm::vec2 size_) {
		if (size_ != _rect.size) {
			InvalidateChildren();
			_rect.SetSize(size_);
			transform->Invalidate();
			update();
		}
	}

	void RectTransform::init() {
		if (transform->parent != nullptr) {
			parentRectTransform = transform->parent->rectTransform;
			isRectTransformChild = transform->parent->isRectTransform;
		}
		renderMode = transform->entity->canvas != nullptr ? transform->entity->canvas->renderMode : RenderMode::WORLD;
	}

	void RectTransform::Invalidate() {
		if (!rectInvalid) {
			rectInvalid = true;
		}

		InvalidateChildren();
	}

	void RectTransform::InvalidateChildren() {
		for (Transform* child : transform->children) {
			if (child->isRectTransform)
				child->rectTransform->Invalidate();
		}
	}

	void RectTransform::render() {
		if (Debug::enabled) {
			Color color;
			glm::vec3 position = Transform::VEC3_ZERO;
			if (renderMode == RenderMode::WORLD) {
				color = { 0.1f, 0.1f, 0.5f, 0.6f };
			}
			else if (renderMode == RenderMode::CAMERA) {
				color = { 0.1f, 0.5f, 0.5f, 0.3f };
			}
			else if (renderMode == RenderMode::SCREEN) {
				color = { 0.5f, 0.0f, 0.1f, 0.3f };
				position = transform->position;
			}

			if (Debug::drawRectTransforms) {
				Design::DrawRect(position, rect, color, 3, DrawMode::HOLLOW, renderMode, transform->MVP);
			}

			if (Debug::drawTransforms) {
				Design::DrawVector(position, transform->up / 3.0f, color::green, 2, false, renderMode, transform->MVP);
				Design::DrawVector(position, transform->right / 3.0f, color::red, 2, false, renderMode, transform->MVP);
				Design::DrawVector(position, transform->forward / 3.0f, color::blue, 2, false, renderMode, transform->MVP);
			}
		}
	}
}  // namespace se