#include "sepch.h"

#include "Renderer.h"

namespace se {

	void Renderer::Copy(Renderer* other) {
		if (other == nullptr) return;
		Component::Copy(other);

		enabled = other->enabled;
		overlay = other->overlay;
		isVisible = other->isVisible;
	}

	void Renderer::OnBecameInvisible() {
		//ctor
	}

	void Renderer::OnBecameVisible() {
		//ctor
	}
}
