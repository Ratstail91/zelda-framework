#pragma once

#include <SDL2/SDL_mixer.h>

#include <string>

class AudioMixer {
public:
	static AudioMixer& GetSingleton();

	void Init();
	void Quit();

	void LoadMusic(std::string);
	void FreeMusic();
	void PlayMusic();
	void StopMusic();
	void PauseMusic();
	void UnpauseMusic();

	bool LoadedMusic();
	bool PlayingMusic();
	bool PausedMusic();

	void FadeMusicIn(int ms);
	void FadeMusicOut(int ms);
	void FadeMusicTo(std::string, int outMs, int inMs);

private:
	//for fading (bugfix blocking)
	friend void fadeMiddle();
	Mix_Music* second;
	int inMilliseconds;

	static AudioMixer singleton;
	Mix_Music* music;
};