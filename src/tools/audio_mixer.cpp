#include "audio_mixer.hpp"

#include <sstream>
#include <stdexcept>

AudioMixer AudioMixer::singleton;

static void error(std::string str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

AudioMixer& AudioMixer::GetSingleton() {
	return singleton;
}

void AudioMixer::Init() {
	//Initialize SDL_mixer
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 8, 4096) != 0) {
		error("Failed to open audio");
	}

	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
		error("Failed to initialize OGG format");
	}
}

void AudioMixer::Quit() {
	Mix_Quit();
	Mix_CloseAudio();
}

void AudioMixer::LoadMusic(std::string fname) {
	FreeMusic();

	music = Mix_LoadMUS(fname.c_str());

	if (music == nullptr) {
		error(std::string() + "Failed to load music file " + fname);
	}
}

void AudioMixer::FreeMusic() {
	Mix_FreeMusic(music);
	music = nullptr;
}

void AudioMixer::PlayMusic() {
	if (Mix_PlayMusic(music, 0) != 0) {
		error("Failed to play music");
	}
}

void AudioMixer::StopMusic() {
	Mix_HaltMusic();
}

void AudioMixer::PauseMusic() {
	Mix_PauseMusic();
}

void AudioMixer::UnpauseMusic() {
	Mix_ResumeMusic();
}

bool AudioMixer::LoadedMusic() {
	return music != nullptr;
}

bool AudioMixer::PlayingMusic() {
	return Mix_PlayingMusic();
}

bool AudioMixer::PausedMusic() {
	//is playing but paused
	return Mix_PlayingMusic() && Mix_PausedMusic();
}

void AudioMixer::FadeMusicIn(int ms) {
	if (Mix_FadeInMusic(music, -1, ms) != 0) {
		error("Failed to fade in music");
	}
}

void AudioMixer::FadeMusicOut(int ms) {
	Mix_FadeOutMusic(ms);
}

//friend function
void fadeMiddle() {
	AudioMixer::GetSingleton().FreeMusic();
	Mix_HookMusicFinished(nullptr);

	AudioMixer::GetSingleton().music = AudioMixer::GetSingleton().second;
	AudioMixer::GetSingleton().FadeMusicIn(AudioMixer::GetSingleton().inMilliseconds);
}

void AudioMixer::FadeMusicTo(std::string fname, int outMs, int inMs) {
	//load second file
	second = Mix_LoadMUS(fname.c_str());

	if (second == nullptr) {
		error(std::string() + "Failed to load music file " + fname);
	}

	Mix_FadeOutMusic(outMs);

	Mix_HookMusicFinished(fadeMiddle);

	//for use in fadeMiddle()
	inMilliseconds = inMs;
}
