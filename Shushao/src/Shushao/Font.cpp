#include "sepch.h"

#include <glad/glad.h>

#include "Resources.h"
#include "Core.h"
#include "Utility.h"
#include "Font.h"
#include "GLManager.h"

namespace se {

	Font::Font(std::string filename, std::string n) {
		if (filename != "") Load(filename);
		name = (n == "" ? util::basename(filename) : n);
	}

	Font::~Font() {
		delete (face);
	}

	void Font::SetSize(float wsize_) {
		SetPixelSize(wsize_ * Config::pixelPerUnit);
	}

	void Font::SetPixelSize(int size_) {
		_size = size_;
		FT_Set_Pixel_Sizes(face, 0, _size);
	}

	bool Font::Load(std::string filename) {
		if (FT_New_Face(GLManager::lFreetype, filename.c_str(), 0, &face)) {
			Debug::Log(ERROR) << "Could not load font: " << filename << std::endl;
			return false;
		}
		init();
		return true;
	}

	bool Font::LoadEmbedded(int IDRES, std::string library) {
		std::vector<char> data = Resources::GetEmbeddedData(IDRES, library, RT_FONT);
		//bytes = (FT_Byte*)(data.data());
		if (bytes != nullptr) delete (bytes);
		bytes = new FT_Byte[data.size()];
		std::copy(data.begin(), data.end(), bytes);
		FT_Error r = FT_New_Memory_Face(GLManager::lFreetype, bytes, data.size() * sizeof(char), 0, &face);
		if (r == 0) init();
		return r == 0;
	}

	void Font::init() {
		int result = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
		if (result > 0) {
			Debug::Log(WARNING) << "Impossible to select charmap in font " << name << std::endl;
		}
	}

	bool Font::LoadCharTexture(const wchar_t p) {
		/* if (FT_Load_Char(font->face, *p, FT_LOAD_RENDER)) {
				return false
			} */
		unsigned long c = FT_Get_Char_Index(face, p);
		if (FT_Load_Glyph(face, c, FT_LOAD_RENDER)) {
			return false;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		return true;
	}

}  // namespace se
