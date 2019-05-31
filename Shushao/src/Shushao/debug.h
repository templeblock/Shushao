#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include "Core.h"
#include "Logger.h"
#include "sepch.h"

namespace se {

enum class GridMode {
    ORTHOGRAFIC,
    PERSPECTIVE
};

class Debug {
public:
    static void Init();

    static bool enabled;
    static bool infoEnabled;
    static bool gridEnabled;
    static bool drawTransforms;
    static bool drawRectTransforms;
    static GridMode debugGridMode;
    static DebugLevel level;
    static Logger Log;

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }

private:
    static std::shared_ptr<spdlog::logger> coreLogger;
    static std::shared_ptr<spdlog::logger> clientLogger;
};

}  // namespace se

// Core log macros
#define DEBUG_CORE_TRACE(...) ::se::Debug::GetCoreLogger()->trace(__VA_ARGS__)
#define DEBUG_CORE_INFO(...) ::se::Debug::GetCoreLogger()->info(__VA_ARGS__)
#define DEBUG_CORE_WARN(...) ::se::Debug::GetCoreLogger()->warn(__VA_ARGS__)
#define DEBUG_CORE_ERROR(...) ::se::Debug::GetCoreLogger()->error(__VA_ARGS__)
#define DEBUG_CORE_FATAL(...) ::se::Debug::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define DEBUG_TRACE(...) ::se::Debug::GetClientLogger()->trace(__VA_ARGS__)
#define DEBUG_INFO(...) ::se::Debug::GetClientLogger()->info(__VA_ARGS__)
#define DEBUG_WARN(...) ::se::Debug::GetClientLogger()->warn(__VA_ARGS__)
#define DEBUG_ERROR(...) ::se::Debug::GetClientLogger()->error(__VA_ARGS__)
#define DEBUG_FATAL(...) ::se::Debug::GetClientLogger()->fatal(__VA_ARGS__)
