#include "sepch.h"

#include "Audio.h"
#include "Debug.h"
#include "Utility.h"

namespace se {

	//{ #region music

	Music::Music(std::string filename) {
		if (filename != "") Load(filename);
		name = util::basename(filename);
	}

	Music::~Music() {
		if (track != nullptr) {
			//Mix_FreeMusic(track);
			track = nullptr;
			//delete(track);
		}
	}

	Music* Music::Load(std::string filename) {
		// track = Mix_LoadMUS(filename.c_str());

		if (track == nullptr) {
			//Debug::Log(ERROR) << "Impossibile caricare " << filename << ": " << Mix_GetError() << std::endl;
			return nullptr;
		}

		return this;
	}

	void Music::play(int loops) {
		if (track == nullptr) return;
		//if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
		//Mix_PlayMusic(track, loops);
		if (!muted) {
			//volume = Mix_VolumeMusic(-1);
			muted = false;
		}
	}

	void Music::resume() {
		//Mix_ResumeMusic();
	}

	void Music::pause() {
		//Mix_PauseMusic();
	}

	void Music::stop() {
		//Mix_HaltMusic();
	}

	void Music::rewind() {
		//Mix_RewindMusic();
	}

	void Music::fadeIn(int loops, int ms) {
		if (track == nullptr) return;
		//if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
		//Mix_FadeInMusic(track, loops, ms);
		muted = false;
	}

	void Music::fadeOut(int ms) {
		//Mix_FadeOutMusic(ms);
	}

	bool Music::isPlaying() {
		return 0;
		//return Mix_PlayingMusic() == 1 && Mix_PausedMusic() != 1;
	}

	bool Music::isPaused() {
		return 0;
		//return Mix_PausedMusic() == 1 && Mix_PlayingMusic() == 1;
	}

	bool Music::isFading() {
		return 0;
		//return Mix_FadingMusic() == 1;
	}

	bool Music::isMuted() {
		return muted;
	}

	void Music::seek(double position) {
		rewind();
		//Mix_SetMusicPosition(position);
	}

	int Music::setVolume(int volume_) {  // 0 - 128
		return 0;
		//Mix_VolumeMusic(volume_);
		//return volume = Mix_VolumeMusic(-1);
	}

	int Music::addVolume(int volume_) {
		return 0;
		//setVolume(Mix_VolumeMusic(-1) + volume_);
		//return volume = Mix_VolumeMusic(-1);
	}

	int Music::volumeDown(int) {
		return 0;
	}

	int Music::getVolume() {
		return 0;
		//return Mix_VolumeMusic(-1);
	}

	void Music::mute() {
		//Mix_VolumeMusic(0);
		muted = true;
	}

	void Music::unMute() {
		//Mix_VolumeMusic(volume);
		muted = false;
	}

	bool Music::muted;
	int Music::volume;

	//}

	//{ #region effect

	Effect::Effect(std::string filename) {
		if (filename != "") Load(filename);
		name = util::basename(filename);
	}

	Effect::~Effect() {
		if (sample != nullptr) {
			//Mix_FreeChunk(sample);
			sample = nullptr;
		}
	}

	Effect* Effect::Load(std::string filename) {
		//sample = Mix_LoadWAV(filename.c_str());

		if (sample == nullptr) {
			//Debug::Log(ERROR) << "Impossibile caricare " << filename << ": " << Mix_GetError() << std::endl;
			return nullptr;
		}

		return this;
	}

	void Effect::setVolume(int volume_) {
		volume = glm::clamp(volume_, 0, 128);
	}

	void Effect::play(int loops) {
		if (sample == nullptr) return;
		//Mix_Volume(channel = Mix_PlayChannel(-1, sample, loops), volume);
	}

	void Effect::play(int loops, int ticks) {
		if (sample == nullptr) return;
		//Mix_Volume(channel = Mix_PlayChannelTimed(-1, sample, loops, ticks), volume);
	}

	void Effect::fadeIn(int loops, int ms) {
		if (sample == nullptr) return;
		//Mix_Volume(channel = Mix_FadeInChannel(-1, sample, loops, ms), volume);
	}

	void Effect::fadeIn(int loops, int ms, int ticks) {
		if (sample == nullptr) return;
		//Mix_Volume(channel = Mix_FadeInChannel(-1, sample, loops, ms), volume);
	}

	void Effect::stop() {
		//if (Mix_GetChunk(channel) == sample && Mix_Playing(channel)) {
		//	Mix_HaltChannel(channel);
		//}
	}

	//}
}  // namespace se
