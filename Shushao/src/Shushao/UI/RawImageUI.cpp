#include "sepch.h"

#include "RawImageUI.h"
#include "Shushao/RawImage.h"

namespace se {
	namespace ui {

		void RawImageUI::Awake() {
			transform->isRectTransform = true;
			AddComponent<se::RawImage>();
		}

	}  // namespace ui
}  // namespace se
