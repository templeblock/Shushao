#include "sepch.h"

#include "Utility.h"
#include "canvas.h"
#include "Color.h"
#include "Design.h"
#include "SceneManager.h"
#include "Transform.h"

namespace se {

	void Transform::setup() {
		rectTransform = new RectTransform(this);
	}

	Transform::~Transform() {
		delete (rectTransform);
		rectTransform = nullptr;
	}

	void Transform::Copy(Transform* other) {
		if (other == nullptr) return;
		Component::Copy(other);

		isRoot = other->isRoot;
		isFree = other->isFree;
		origin = other->origin;
		parent = other->parent;
		localPosition = other->localPosition;
		localRotation = other->localRotation;
		localScale = other->localScale;
		matrixInvalid = other->matrixInvalid;
		inverseMatrixInvalid = other->inverseMatrixInvalid;
		_position = other->position;
		_rotation = other->rotation;
		_scale = other->scale;
		_velocity = other->velocity;
		_forward = other->forward;
		_right = other->right;
		_up = other->up;
		_pivot = other->pivot;
		_MVP = other->MVP;
		_MV = other->MV;
		_VP = other->VP;
		_M = other->M;
		_V = other->V;
		_P = other->P;
		_localToWorldMatrix = other->localToWorldMatrix;
		_localToParentMatrix = other->localToParentMatrix;
		_worldToLocalMatrix = other->worldToLocalMatrix;
		_rootMatrix = other->rootMatrix;
		lastPosition = other->lastPosition;
	}

	//{ #region parenting

	Transform* Transform::GetParent() {
		return parent;
	}

	bool Transform::isAtRoot() {
		return (parent == nullptr || isRoot || (parent != nullptr && parent->isRoot));
	}

	void Transform::SetParent(Transform* newpa, bool worldPositionStays) {
		if (parent != nullptr) {
			parent->RemoveChild(this);
		}

		parent = newpa;
		if (parent->entity->canvas != nullptr) {
			entity->canvas = parent->entity->canvas;
		}
		else if (entity->GetComponent<Canvas>() == nullptr) {
			entity->canvas = nullptr;
		}

		rectTransform->init();

		if (worldPositionStays) {
			localPosition = _position - parent->position;
		}
		else {
			localPosition = VEC3_ZERO;
		}

		parent->AddChild(this);
		matrixInvalid = true;
	}

	void Transform::RemoveChild(Transform* t) {
		if (children.empty()) return;
		auto it = find(children.begin(), children.end(), t);
		if (it != children.end()) {
			children.erase(it);
		}
	}

	void Transform::AddChild(Transform* t) {
		if (children.empty()) {
			children.push_back(t);
			return;
		}
		auto it = find(children.begin(), children.end(), t);
		if (it == children.end()) children.push_back(t);
	}

	Transform* Transform::GetChild(unsigned int index) {
		if (index >= children.size()) return nullptr;
		return children.at(index);
	}

	std::vector<Transform*> Transform::GetParents() {
		std::vector<Transform*> parents;
		if (isAtRoot()) return parents;

		Transform* p = parent;
		while (p != nullptr && !p->isRoot) {
			parents.push_back(p);
			p = p->GetParent();
		}

		return parents;
	}

	//}

	void Transform::Invalidate() {
		if (!matrixInvalid) {
			matrixInvalid = true;
			inverseMatrixInvalid = true;
		}

		for (Transform* child : children) {
			child->Invalidate();
		}
	}

	//{ #region position

	void Transform::SetPosition(glm::vec3 position_) {
		if (isRoot) return;
		origin = Origin::WORLD;
		Invalidate();
		localPosition = isAtRoot() ? position_ : parent->position - position_;
		_position = position_;

		if (isRectTransform) {
			rectTransform->update();
		}

		setupDirections();
	}

	void Transform::SetLocalPosition(glm::vec3 position_) {
		if (isRoot) return;
		origin = Origin::LOCAL;
		localPosition = position_;
		Invalidate();
		_position = GetWorldPosition();

		if (isRectTransform) {
			rectTransform->update();
		}

		setupDirections();
	}

	void Transform::SetLocalRotation(glm::vec3 angles_) {  // angles_ is in radians
		if (isRoot) return;
		origin = Origin::LOCAL;
		localRotation = glm::quat(angles_ * DEGTORAD);
		Invalidate();
		_rotation = GetWorldOrientation();
		setupDirections();
	}

	void Transform::SetLocalRotation(glm::quat rotation_) {
		if (isRoot) return;
		origin = Origin::LOCAL;
		localRotation = rotation_;
		Invalidate();
		_rotation = GetWorldOrientation();
		setupDirections();
	}

	void Transform::SetRotation(glm::vec3 angles_) {  // angles_ is in radians
		if (isRoot) return;
		origin = Origin::WORLD;
		_rotation = glm::quat(angles_ * DEGTORAD);
		//localRotation = _rotation;
		setupDirections();
	}

	void Transform::SetRotation(glm::quat rotation_) {
		if (isRoot) return;
		origin = Origin::WORLD;
		_rotation = rotation_;
		Invalidate();
		//localRotation = _rotation * parent->rotation;
		setupDirections();
	}

	glm::vec3 Transform::GetEulerAngles() {
		return glm::eulerAngles(rotation) * RADTODEG;
	}

	glm::vec3 Transform::GetLocalEulerAngles() {
		return glm::eulerAngles(localRotation) * RADTODEG;
	}

	void Transform::setupDirections() {
		_up = _rotation * UP;
		_right = _rotation * RIGHT;
		_forward = _rotation * FORWARD;
	}

	void Transform::SetLocalScale(glm::vec3 scale_) {
		if (isRoot) return;
		localScale = scale_;
		_scale = GetWorldScale();
	}

	void Transform::SetPivot(glm::vec2 pivot_) {
		if (isRoot) return;
		_pivot = { pivot_.x, pivot_.y, 0.0f };
		rectTransform->SetPivot({ pivot.x, pivot.y });
	}

	//}

	void Transform::buildMVP() {
		_M = GetLocalToWorldMatrix();
		if (entity->canvas != nullptr && entity->canvas->renderMode == RenderMode::CAMERA && entity->canvas->camera != nullptr) {
			_P = entity->canvas->camera->getProjectionMatrix();
			_V = entity->canvas->camera->getViewMatrix();
		}
		else {
			_P = SceneManager::activeScene->activeCamera->getProjectionMatrix();
			_V = SceneManager::activeScene->activeCamera->getViewMatrix();
		}
		_MVP = _P * _V * _M;
		_MV = _V * _M;
		_VP = _P * _V;
	}

	float* Transform::uMVP() {
		return &_MVP[0][0];
	}

	float* Transform::uM() {
		return &_M[0][0];
	}

	float* Transform::uP() {
		return &_P[0][0];
	}

	float* Transform::uV() {
		return &_V[0][0];
	}

	float* Transform::uMV() {
		return &_MV[0][0];
	}

	float* Transform::uVP() {
		return &_VP[0][0];
	}

	void Transform::Awake() {
		buildMVP();
		Invalidate();
		rectTransform->init();
	}

	void Transform::Update() {
		if (origin == Origin::LOCAL) {
			_position = GetWorldPosition();
			_rotation = GetWorldOrientation();
		}

		if (isRectTransform) {
			rectTransform->update();
		}

		setupDirections();
		buildMVP();

		_velocity = (1 / Time::deltaTime) * (position - lastPosition);
		lastPosition = position;
	}

	void Transform::Render() {
		if (isRectTransform) {
			rectTransform->render();
		}
		else {
			if (Debug::enabled && Debug::drawTransforms) {
				Design::DrawVector(VEC3_ZERO, up / 3.0f, color::green, 2, false, RenderMode::WORLD, MVP);
				Design::DrawVector(VEC3_ZERO, right / 3.0f, color::red, 2, false, RenderMode::WORLD, MVP);
				Design::DrawVector(VEC3_ZERO, forward / 3.0f, color::blue, 2, false, RenderMode::WORLD, MVP);
			}
		}
	}

	glm::mat4 Transform::GetLocalToParentMatrix() {
		if (isRectTransform && parent->isRectTransform) {
			_localToParentMatrix = rectTransform->GetLocalToParentMatrix();
		}
		else {
			_localToParentMatrix = glm::translate(glm::mat4(), localPosition) * glm::toMat4(localRotation) * glm::scale(glm::mat4(), localScale);
		}
		return _localToParentMatrix;
	}

	glm::mat4 Transform::GetRootMatrix() {
		return glm::translate(glm::mat4(), _position) * glm::toMat4(_rotation) * glm::scale(glm::mat4(), localScale);
	}

	glm::mat4 Transform::GetLocalToWorldMatrix() {
		if (matrixInvalid) {
			if (origin == Origin::WORLD) {
				_localToWorldMatrix = GetRootMatrix();
			}
			else if (isAtRoot()) {
				_localToWorldMatrix = GetLocalToParentMatrix();
			}
			else {
				_localToWorldMatrix = parent->GetLocalToWorldMatrix() * GetLocalToParentMatrix();
			}
			matrixInvalid = false;
		}
		return _localToWorldMatrix;
	}

	glm::mat4 Transform::GetWorldToLocalMatrix() {
		/*if (inverseMatrixInvalid) {
				_worldToLocalMatrix = glm::inverse(GetLocalToWorldMatrix());
				inverseMatrixInvalid = false;
			}
			return _worldToLocalMatrix;*/

		if (inverseMatrixInvalid) {
			if (isAtRoot()) {
				_worldToLocalMatrix = glm::mat4();
			}
			else {
				_worldToLocalMatrix = glm::toMat4(parent->rotation) * glm::translate(glm::mat4(), -parent->position);
			}
			inverseMatrixInvalid = false;
		}
		return _worldToLocalMatrix;
	}

	glm::vec3 Transform::GetWorldPosition() {
		glm::vec4 p = GetLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return glm::vec3(p.x, p.y, p.z);
	}

	glm::vec3 Transform::GetLocalPosition(glm::vec3 position_) {
		glm::vec4 p = GetWorldToLocalMatrix() * glm::vec4(position_.x, position_.y, position_.z, 1.0f);
		return glm::vec3(p.x, p.y, p.z);
	}

	glm::vec3 Transform::GetWorldScale() {
		if (isAtRoot()) {
			return localScale;
		}
		else {
			return localScale * parent->GetWorldScale();
		}
	}

	glm::quat Transform::GetWorldOrientation() {
		if (isAtRoot()) {
			return localRotation;
		}
		else {
			return localRotation * parent->rotation;
		}
		return glm::quat();
	}

	const glm::mat4 Transform::MAT4_IDENTITY = glm::mat4();
	const glm::vec3 Transform::VEC3_ZERO = { 0.0f, 0.0f, 0.0f };
	const glm::vec3 Transform::VEC3_IDENTITY = { 1.0f, 1.0f, 1.0f };
	const glm::vec3 Transform::VEC3_IDENTITY2D = { 1.0f, 1.0f, 0.0f };
	const glm::vec3 Transform::UP = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 Transform::AXIS_X = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 Transform::AXIS_Y = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 Transform::AXIS_Z = { 0.0f, 0.0f, 1.0f };
	const glm::vec3 Transform::DOWN = { 0.0f, -1.0f, 0.0f };
	const glm::vec3 Transform::FORWARD = { 0.0f, 0.0f, 1.0f };
	const glm::vec3 Transform::BACK = { 0.0f, 0.0f, -1.0f };
	const glm::vec3 Transform::RIGHT = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 Transform::LEFT = { -1.0f, 0.0f, 0.0f };
	const glm::quat Transform::QUATERNION_IDENTITY = { 1.0f, 0.0f, 0.0f, 0.0f };

}  // namespace se
