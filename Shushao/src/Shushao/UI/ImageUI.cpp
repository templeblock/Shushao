#include "sepch.h"

#include "ImageUI.h"
#include "Shushao/Image.h"

namespace se {
	namespace ui {

		void ImageUI::Awake() {
			transform->isRectTransform = true;
			AddComponent<se::Image>();
		}
	}  // namespace ui
}  // namespace se
