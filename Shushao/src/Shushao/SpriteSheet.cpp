#include "sepch.h"

#include "SpriteSheet.h"
#include "Resources.h"
#include "Utility.h"
#include "Rect.h"
#include "TextureAtlas.h"

namespace se {

	SpriteSheet::SpriteSheet(TextureAtlas* texture) {
		name = "Spritesheet " + texture->name;
		Load(texture);
	}

	SpriteSheet::SpriteSheet(std::string _name, TextureAtlas* texture) {
		name = _name;
		Load(texture);
	}

	SpriteSheet::SpriteSheet(std::string _name) {
		name = _name;
	}

	SpriteSheet::~SpriteSheet() {
		// le risorse sono distrutte dal processo madre
		clear();
	}

	Sprite* SpriteSheet::next() {
		if (++cursor >= (int)size()) cursor = 0;
		return (*this)[cursor];
	}

	Sprite* SpriteSheet::prev() {
		if (--cursor < 0) cursor = (int)size() - 1;
		return (*this)[cursor];
	}

	void SpriteSheet::Add(Sprite* sprite) {
		push_back(sprite);
	}

	void SpriteSheet::Load(TextureAtlas* atlas) {
		if (atlas == nullptr) return;

		int counter = 0;

		if (name == "Spritesheet") {
			name = atlas->name;
		}

		if (atlas->slices.size() == 0) {
			return;
		}

		for (const std::pair<Rect, Align>& slice : atlas->slices) {
			Rect rect = slice.first;
			Align pivotpos = slice.second;

			Sprite* sprite = new Sprite(name + "_" + ts(counter), atlas);
			sprite->SetRect(rect);
			sprite->SetPivot(pivotpos);
			sprite->Build();

			if (sprite != nullptr) {
				Resources::Add(sprite);
				push_back(sprite);
				counter++;
			}
		}
	}

}  // namespace se
