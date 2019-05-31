#ifndef SE_AUDIO
#define SE_AUDIO

#include "Shushao/Core.h"
#include "sepch.h"

#include "Object.h"

namespace se {

	class Music : public Object {
	public:
		Music(std::string filename = "");
		~Music();

		// Mix_Music* track;
		void* track;

		Music* Load(std::string);

		void play(int loops = 1);
		void fadeIn(int loops = 1, int ms = 1000);

		// STATIC

		static void resume();
		static void pause();
		static void stop();
		static void rewind();
		static void fadeOut(int ms = 1000);
		static void seek(double position);

		static int setVolume(int);
		static int addVolume(int);
		static int volumeDown(int);
		static int getVolume();
		static void mute();
		static void unMute();

		static bool isPlaying();
		static bool isPaused();
		static bool isFading();
		static bool isMuted();

	private:
		static bool muted;
		static int volume;
	};

	class Effect : public Object {
		// TODO: PANNING

	public:
		Effect(std::string filename = "");
		~Effect();

		// Mix_Chunk* sample;
		void* sample;

		Effect* Load(std::string);

		void setVolume(int);

		void play(int loops = 0);
		void play(int loops, int ticks);
		void fadeIn(int loops, int ms = 1000);
		void fadeIn(int loops, int ms, int ticks);
		void stop();

	private:
		int volume = 128;
		int channel;
	};

}  // namespace se

#endif