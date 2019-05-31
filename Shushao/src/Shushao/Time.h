#ifndef SE_TIME
#define SE_TIME

#include "Shushao/Core.h"
#include "sepch.h"

#include "Utility.h"
#include "Config.h"

namespace se {

class Time {
public:
    static float captureFramerate;  // todo
    static float deltaTime;
    static float fixedDeltaTime;
    static float fixedTime;
    static float fixedUnscaledDeltaTime;  // todo
    static float fixedUnscaledTime;  // todo
    static float frameCount;
    static bool inFixedTimeStep;
    static float maximumDeltaTime;  // todo
    static float maximumParticleDeltaTime;  // todo
    static float realtimeSinceStartup;
    static float smoothDeltaTime;  // todo
    static float time;
    static float timeScale;  // todo
    static float timeSinceLevelLoad;  // todo
    static float unscaledDeltaTime;  // todo
    static float unscaledTime;  // todo

    static float frameRateLimit;
    static float fixedRateLimit;
    static float renderTime;
    static float frameRate;
    static float renderDeltaTime;
    static float frameLimitDuration;
    static float fixedLimitDuration;

    static void setFrameRateLimit(float);
    static void setFixedRateLimit(float);

    static float GetTime();
    static void Update();
    static std::string Clock();

private:
    static std::chrono::steady_clock::time_point startTime;
};

}  // namespace se

#endif