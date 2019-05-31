#include "sepch.h"

#include "PanelUI.h"
#include "Shushao/Image.h"

namespace se {
	namespace ui {

		void PanelUI::Awake() {
			transform->isRectTransform = true;
			AddComponent<se::Image>();
		}
	}  // namespace ui
}  // namespace se
