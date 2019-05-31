#include "sepch.h"

#include "Debug.h"
#include "TextureAtlas.h"

namespace se {

	void TextureAtlas::AddSlice(Rect _rect, Align _Align) {
		if (TextureID == 0) {
			Debug::Log(WARNING, SOURCE) << "No texture loaded" << std::endl;
			return;
		}
		slices.push_back({ _rect, _Align });
	}

	void TextureAtlas::AddGrid(glm::vec2 _size, Align _pivotPos, glm::vec2 _offset, glm::vec2 _padding) {
		if (TextureID == 0) {
			Debug::Log(WARNING, SOURCE) << "No texture loaded" << std::endl;
			return;
		}

		glm::vec2 cursor = glm::vec2(0, 0) + _offset;
		glm::vec2 step = _size + _padding;

		while (cursor.y < height) {
			while (cursor.x < width) {
				Rect slice;
				slice.Set(cursor.x + _padding.x, cursor.y + _padding.y, _size.x, _size.y);
				AddSlice(slice, _pivotPos);
				cursor.x += step.x;
			}
			cursor.y += step.y;
		}
	}
}  // namespace se
