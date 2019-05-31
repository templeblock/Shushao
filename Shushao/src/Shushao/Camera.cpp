#include "sepch.h"

#include "Config.h"
#include "Core.h"
#include "Debug.h"
#include "GLmanager.h"
#include "Transform.h"
#include "Camera.h"

namespace se {

void Camera::Copy(Camera* other) {
    if (other == nullptr) return;
    Component::Copy(other);

    depth = other->getDepth();
    orthographic = other->getOrthographic();
    orthographicSize = other->getOrthographicSize();
    fieldOfView = other->getFieldOfView();
    farClipPlane = other->getFarClipPlane();
    nearClipPlane = other->getNearClipPlane();
    viewportSize = other->getViewportSize();
    rect = other->getRect();
}

void Camera::setup() {
    Projection = getProjectionMatrix();
}

glm::mat4 Camera::getProjectionMatrix() {
    if (aspect != GLManager::ASPECT ||
        orthographicSize != last_orthographicSize ||
        nearClipPlane != last_nearClipPlane ||
        farClipPlane != last_farClipPlane ||
        fieldOfView != last_fieldOfView) {
        aspect = GLManager::ASPECT;
        last_orthographicSize = orthographicSize;
        last_nearClipPlane = nearClipPlane;
        last_farClipPlane = farClipPlane;
        last_fieldOfView = fieldOfView;

        if (orthographic) {
            Projection = glm::ortho(
                -orthographicSize * aspect,
                orthographicSize * aspect,
                -orthographicSize,
                orthographicSize,
                nearClipPlane,
                farClipPlane);
            viewportSize = {orthographicSize * aspect * 2, orthographicSize * 2};
        } else {
            Projection = glm::perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
        }
    }

    return Projection;
}

glm::mat4 Camera::getViewMatrix() {
    return lookAt(transform->localPosition, Transform::FORWARD, Transform::UP);
}

float Camera::getDepth() {
    return depth;
}

bool Camera::getOrthographic() {
    return orthographic;
}

float Camera::getOrthographicSize() {
    return orthographicSize;
}

float Camera::getFieldOfView() {
    return fieldOfView;
}

float Camera::getFarClipPlane() {
    return farClipPlane;
}

float Camera::getNearClipPlane() {
    return nearClipPlane;
}

Rect Camera::getRect() {
    return rect;
}

void Camera::setDepth(float value) {
    depth = value;
    Projection = getProjectionMatrix();
}

void Camera::setOrthographic(bool value) {
    orthographic = value;
    Projection = getProjectionMatrix();
}

void Camera::setOrthographicSize(float value) {
    orthographicSize = value;
    Projection = getProjectionMatrix();
}

void Camera::setFieldOfView(float value) {
    fieldOfView = value;
    Projection = getProjectionMatrix();
}

void Camera::setFarClipPlane(float value) {
    farClipPlane = value;
    Projection = getProjectionMatrix();
}

void Camera::setNearClipPlane(float value) {
    nearClipPlane = value;
    Projection = getProjectionMatrix();
}

void Camera::setRect(Rect value) {
    rect = value;
    Projection = getProjectionMatrix();
}

void Camera::print() {
    Logger::setColor(ConsoleColor::CYAN);

    std::cout << " Camera " << name << std::endl;

    std::cout << "  - "
              << "backgroundColor: " << backgroundColor.r << " " << backgroundColor.g << " " << backgroundColor.b << " " << backgroundColor.a << std::endl;
    std::cout << "  - "
              << "aspect: " << aspect << std::endl;
    std::cout << "  - "
              << "orthographic: " << (orthographic ? "true" : "false") << std::endl;
    std::cout << "  - "
              << "orthographicSize: " << orthographicSize << std::endl;
    std::cout << "  - "
              << "depth: " << depth << std::endl;
    std::cout << "  - "
              << "fieldOfView: " << fieldOfView << std::endl;
    std::cout << "  - "
              << "farClipPlane: " << farClipPlane << std::endl;
    std::cout << "  - "
              << "nearClipPlane: " << nearClipPlane << std::endl;
    std::cout << "  - "
              << "rect: " << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << std::endl;

    Logger::setColor(ConsoleColor::LIGHTGREY);
}
}  // namespace se
