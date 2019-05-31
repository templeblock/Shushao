#ifndef SE_TEXTUREATLAS
#define SE_TEXTUREATLAS

#include "Core.h"
#include "sepch.h"

#include "Rect.h"
#include "Sprite.h"
#include "Texture.h"

namespace se {

class TextureAtlas : public Texture {
public:
    TextureAtlas(){};
    TextureAtlas(std::string filename) : Texture(filename){};
    TextureAtlas(std::string filename, std::string n) : Texture(filename, n){};

    std::vector<std::pair<Rect, Align>> slices;

    void AddSlice(Rect, Align pp = Align::CENTER);

    void AddGrid(
        glm::vec2,
        Align pp = Align::CENTER,
        glm::vec2 = glm::vec2(0, 0),
        glm::vec2 = glm::vec2(0, 0));
};

}  // namespace se

#endif