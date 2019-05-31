#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shushao/Core.h"
#include "Window.h"
#include "sepch.h"

namespace se {

class GLManager {
public:
    static FT_Library lFreetype;

    static bool fullscreen;
    static bool ready;
    static unsigned int DESKTOP_WIDTH;
    static unsigned int DESKTOP_HEIGHT;
    static unsigned int WIDTH;
    static unsigned int HEIGHT;
    static float ASPECT;
    static glm::vec2 VIEWPORT;

    static bool Init();
    static void Quit();
    static void Update();
};

}  // namespace se
