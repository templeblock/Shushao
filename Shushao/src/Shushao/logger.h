#pragma once

#include "Core.h"
#include "sepch.h"

#undef ERROR
#undef InvokeMethod

#include "Utility.h"
#include "Time.h"

#define SOURCE util::basename(__FILE__) + "::" + __FUNCTION__ + ":" + util::toString(__LINE__) + ""
#define LOG(MESSAGE) Debug::Log(SOURCE) << MESSAGE << std::endl;

namespace se {

enum DebugLevel {
    INFO,
    WARNING,
    ERROR
};

enum ConsoleColor {
    ZERO,
    DARKBLUE,
    DARKGREEN,
    DARKCYAN,
    DARKRED,
    DARKPURPLE,
    DARKYELLOW,
    LIGHTGREY,
    GREY,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PINK,
    YELLOW,
    WHITE
};

class Logger {
    std::ostringstream stream;
    DebugLevel streamLevel = DebugLevel::INFO;
    bool logStarted = false;
    std::ofstream debugfile;
    std::string sender = "";
    std::string debugFilename = "debug.log";

public:
    Logger(DebugLevel level_);

    DebugLevel level;
    bool enabled = false;

    Logger& operator()(DebugLevel l);
    Logger& operator()(DebugLevel l, std::string s);
    Logger& operator()(std::string s);

    template <typename T>
    Logger& operator<<(T&& value) {
        if (streamLevel < level) return *this;
        stream << value;
        return *this;
    }

    Logger& operator<<(std::ostream& (*os)(std::ostream&));

    static void setColor(ConsoleColor);
};

}  // namespace se
