#include "sepch.h"

#include "CanvasUI.h"

namespace se {
	namespace ui {

		void CanvasUI::Awake() {
			transform->isRectTransform = true;
			canvas = AddComponent<se::Canvas>();
		}
	}  // namespace ui
}  // namespace se
