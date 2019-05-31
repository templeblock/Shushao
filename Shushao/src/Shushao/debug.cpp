#include "sepch.h"

#include "Debug.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace se {

	void Debug::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		coreLogger = spdlog::stdout_color_mt("SHUSHAO");
		coreLogger->set_level(spdlog::level::trace);

		clientLogger = spdlog::stdout_color_mt("APP");
		clientLogger->set_level(spdlog::level::trace);
	}

	bool Debug::enabled = false;
	bool Debug::gridEnabled = true;
	bool Debug::infoEnabled = true;
	bool Debug::drawTransforms = true;
	bool Debug::drawRectTransforms = true;
	GridMode Debug::debugGridMode = GridMode::ORTHOGRAFIC;
	DebugLevel Debug::level = DebugLevel::INFO;
	std::shared_ptr<spdlog::logger> Debug::coreLogger;
	std::shared_ptr<spdlog::logger> Debug::clientLogger;
	Logger Debug::Log(Debug::level);

}  // namespace se
