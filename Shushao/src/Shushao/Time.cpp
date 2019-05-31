#include "sepch.h"

#include "Time.h"

// TODO: problema chrono

namespace se {

	float Time::GetTime() {
		using namespace std::chrono;
		return (float)duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() / 1000.f;
	}

	void Time::setFrameRateLimit(float fr) {
		frameRateLimit = fr;
		frameLimitDuration = 1.f / frameRateLimit;
	}

	void Time::setFixedRateLimit(float fr) {
		fixedRateLimit = fr;
		fixedLimitDuration = 1.f / fixedRateLimit;
	}

	void Time::Update() {
		realtimeSinceStartup = GetTime();
		deltaTime = realtimeSinceStartup - time;
		time = realtimeSinceStartup;
		fixedDeltaTime = realtimeSinceStartup - fixedTime;
		renderDeltaTime = realtimeSinceStartup - renderTime;
		frameRate = 1.0f / deltaTime;
	}

	std::string Time::Clock() {
		using namespace std::chrono;
		int mil = duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count();

		int mills = (int)(mil % 1000);
		int seconds = (int)(mil / 1000) % 60;
		int minutes = (int)((mil / (1000 * 60)) % 60);
		//int hours   = (int) ((mil / (1000*60*60)) % 24);

		std::ostringstream clock;

		// clock << Utility::zerofill(hours, 2) << ":" << Utility::zerofill(minutes, 2) << ":" << Utility::zerofill(seconds, 2)<< "." << Utility::zerofill(mills, 3);
		clock << util::zerofill(minutes, 2) << ":" << util::zerofill(seconds, 2) << "." << util::zerofill(mills, 3);

		return clock.str();
	}

	// static members definitions
	float Time::deltaTime = 0.0f;
	float Time::fixedDeltaTime = 0.0f;
	float Time::renderDeltaTime = 0.0f;
	float Time::fixedTime = 0.0f;
	float Time::frameCount = 0.0f;
	float Time::frameRate = 0.0f;
	float Time::renderTime = 0.0f;
	float Time::time = 0.0f;
	float Time::realtimeSinceStartup = 0.0f;
	bool Time::inFixedTimeStep = false;
	std::chrono::steady_clock::time_point Time::startTime = std::chrono::high_resolution_clock::now();

	// from Config
	float Time::fixedRateLimit = Config::Time::fixedRateLimit;
	float Time::fixedLimitDuration = 1.0f / Config::Time::fixedRateLimit;
	float Time::frameRateLimit = Config::Time::frameRateLimit;
	float Time::frameLimitDuration = 1.0f / Config::Time::frameRateLimit;
}  // namespace se
