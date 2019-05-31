#pragma once

#include "Shushao/Core.h"
#include "sepch.h"

#include "LevelMap.h"
#include "Utility.h"

#define USER_CONFIG_FILE "config/config.ini"

namespace Config {

extern std::string title;
extern std::string startScene;
extern bool debug;

// textures
extern unsigned int pixelPerUnit;

// display
extern float displayAspect;
extern unsigned int displayWidth;
extern unsigned int displayHeight;
extern bool fullscreen;
extern const std::map<std::string, std::pair<int, int>> DisplayModes;

// time
namespace Time {
    extern float fixedRateLimit;  // 0.0f -> unlimited
    extern float frameRateLimit;  // 0.0f -> unlimited
}  // namespace Time

// Physics (box2d)
namespace Physics {
    extern bool enabled;
    extern bool debug;
    extern glm::vec2 gravity;
    extern bool doSleep;
    extern int velocityIterations;
    extern int positionIterations;
}  // namespace Physics

extern se::LevelMap Layers;
extern se::LevelMap SortingLayers;

extern std::map<std::string, std::string> data;
extern bool LoadUserConfig();
extern bool LoadEngineConfig();
extern bool parseUserConfig();
extern bool parseEngineConfig();

template <class T>
bool processConfigData(T& cstring) {
    std::vector<std::string> parts;
    std::string line, f;
    while (getline(cstring, line)) {
        if (line.size() == 0) continue;
        f = line.substr(0, 1);
        if (f == "#" || f == "/" || f == "[" || f == "*" || f == "[") continue;
        std::vector<std::string> parts = se::util::split(line, '=');
        if (parts.size() > 1) {
            //data.insert({parts[0], parts[1]});
            data[parts[0]] = parts[1];
        }
    }
    return true;
}
}  // namespace Config
