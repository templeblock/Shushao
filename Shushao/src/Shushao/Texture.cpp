#include "sepch.h"

#include <SOIL2.h>
#include <glad/glad.h>

#include "Resources.h"
#include "Utility.h"
#include "Texture.h"

namespace se {

	Texture::Texture(std::string filename, std::string n) {
		if (filename != "") Load(filename);
		name = (n == "" ? util::basename(filename) : n);
	}

	Texture::~Texture() {
		const GLuint textures[] = { TextureID };
		glDeleteTextures(1, textures);
	}

	bool Texture::LoadEmbedded(int IDRES, std::string library) {
		std::vector<char> data = Resources::GetEmbeddedData(IDRES, library);
		return LoadBytes(data);
	}

	bool Texture::LoadBytes(std::vector<char> data) {
		//TextureID = SOIL_load_OGL_texture_from_memory(
		//    (const unsigned char*)&data[0],
		//    data.size(),
		//    SOIL_LOAD_AUTO,
		//    SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT  //
		//);

		// method 2
		unsigned char* image = SOIL_load_image_from_memory((const unsigned char*)& data[0], data.size(), &width, &height, 0, SOIL_LOAD_RGB);
		Bind(image);
		SOIL_free_image_data(image);

		return TextureID > 0;

#if 0
		SDL_RWops * rw = SDL_RWFromMem(&data[0], data.size());
		Surface = IMG_LoadPNG_RW(rw);

		if (Surface == nullptr) {
			Debug::Log(WARNING) << "Can't load embedded image '" << name << "': " << IMG_GetError() << std::endl;
			return false;
		}
		return Bind(Surface);
#endif
	}

	bool Texture::Load(std::string filename) {
		// TextureID = SOIL_load_OGL_texture(
		//     filename.c_str(),
		//     SOIL_LOAD_AUTO,
		//     SOIL_CREATE_NEW_ID,
		//     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT  //
		// );

		// method 2
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		Bind(image);
		SOIL_free_image_data(image);

		return TextureID > 0;
#if 0
		Surface = IMG_Load(filename.c_str());
		name = util::basename(filename);

		if (!Surface) {
			Debug::Log(ERROR) << "IMG_Load " << IMG_GetError() << std::endl;
			return false;
		}

		return Bind(Surface);
#endif
	}

	void Texture::Bind(unsigned char* image_) {
		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_);

		// glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/* bool Texture::Bind(SDL_Surface* surface) {
		if (Surface == nullptr) return false;
		width = surface->w;
		height = surface->h;
		size = {width, height};

		int Mode = (surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB);

		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glGenerateMipmap(GL_TEXTURE_2D);

		SDL_FreeSurface(surface);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (TextureID)
			return true;
		else
			return false;
	} */

	unsigned int Texture::GetTextureID() {
		return TextureID;
	}

}  // namespace se
