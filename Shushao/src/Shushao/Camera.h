#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

#include "Color.h"
#include "Component.h"
#include "Rect.h"

namespace se {

	class Camera : public Component {
	public:
		virtual void setup();

		Color backgroundColor;

		// mat4 worldToCameraMatrix; // TODO? Matrix that transforms from world to camera space.
		// mat4 cameraToWorldMatrix; // TODO?

		glm::mat4 View;
		glm::mat4 Projection;

		// methods
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
		void setDepth(float);
		void setOrthographic(bool);
		void setOrthographicSize(float);
		void setFieldOfView(float);
		void setFarClipPlane(float);
		void setNearClipPlane(float);
		void setRect(Rect);
		float getDepth();
		bool getOrthographic();
		float getOrthographicSize();
		float getFieldOfView();
		float getFarClipPlane();
		float getNearClipPlane();
		Rect getRect();
		glm::vec2 getViewportSize() { return viewportSize; }

		void print();

		void Copy(Camera* other);

	private:
		float aspect;
		bool orthographic = true;
		float orthographicSize = 5.0f;
		float depth = 3.0f;
		float fieldOfView = 45.0f;
		float farClipPlane = 1000.0f;
		float nearClipPlane = 3.0f;

		bool last_orthographic = true;
		float last_orthographicSize = 5.0f;
		float last_depth = 3.0f;
		float last_fieldOfView = 45.0f;
		float last_farClipPlane = 1000.0f;
		float last_nearClipPlane = 3.0f;

		glm::vec2 viewportSize = { 0.0f, 0.0f };

		Rect rect;  // Where on the screen is the camera rendered in normalized coordinates.
	};

}  // namespace se
