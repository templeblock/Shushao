
#include "sepch.h"

#include "Shushao/Core.h"
#include "Debug.h"
#include "Utility.h"
#include "Config.h"
#include "Logger.h"

namespace se {

	std::map<DebugLevel, std::string> DebugLevelName = {
		{INFO, "INFO"},
		{WARNING, "WARNING"},
		{ERROR, "ERROR"} };

	Logger::Logger(DebugLevel level_) {
		level = level_;
	}

	Logger& Logger::operator()(DebugLevel debugLevel_) {
		streamLevel = debugLevel_;
		return *this;
	}

	Logger& Logger::operator()(DebugLevel debugLevel_, std::string sender_) {
		streamLevel = debugLevel_;
		sender = sender_;
		return *this;
	}

	Logger& Logger::operator()(std::string sender_) {
		sender = sender_;
		return *this;
	}

	Logger& Logger::operator<<(std::ostream& (*os)(std::ostream&)) {
		if (streamLevel < level || !Debug::enabled) return *this;

		if (streamLevel == INFO) {
			setColor(ConsoleColor::LIGHTGREY);
		}
		else if (streamLevel == WARNING) {
			setColor(ConsoleColor::YELLOW);
		}
		else if (streamLevel == ERROR) {
			setColor(ConsoleColor::RED);
		}

		if (sender != "") sender = "<" + sender + "> ";

		std::ostringstream buildmex;
		buildmex << Time::Clock() << " [" << DebugLevelName[streamLevel] << "] " << sender << stream.str() << os;

		std::cout << buildmex.str();

		std::ios_base::openmode mode = std::ios_base::out;
		if (logStarted) mode = mode | std::ios_base::app;

		if (streamLevel > INFO) {
			debugfile.open(debugFilename, mode);
			debugfile << buildmex.str();
			debugfile.close();
		}

		logStarted = true;

		setColor(ConsoleColor::LIGHTGREY);
		stream.str("");
		streamLevel = DebugLevel::INFO;
		sender = "";

		return *this;
	}

	void Logger::setColor(ConsoleColor color) {
#ifdef CONSOLE_COLORS
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#endif
	}

}  // namespace se
