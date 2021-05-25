#pragma once

#include <SDL2/SDL_mixer.h>

#include <map>
#include <string>

class AudioMixer {
public:
	//singleton pattern
	static AudioMixer& GetSingleton();

	//setup
	void Init();
	void Quit();

	//music controls
	void LoadMusic(std::string);
	void UnloadMusic();
	void PlayMusic();
	void StopMusic();
	void PauseMusic();
	void UnpauseMusic();

	bool GetMusicLoaded();
	bool GetMusicPlaying();
	bool GetMusicPaused();

	void SetMusicVolume(Uint8);
	int GetMusicVolume();

	void FadeMusicIn(int ms);
	void FadeMusicOut(int ms);
	void FadeMusicTo(std::string, int outMs, int inMs);

	//chunk controls
	void LoadChunk(std::string key, std::string fname);
	void UnloadChunk(std::string key);
	int PlayChunk(std::string key, int channel = -1);
	void StopChannel(int i);
	void PauseChannel(int i);
	void UnpauseChannel(int i);

	bool GetChunkLoaded(std::string key);
	bool GetChannelPlaying(int i);
	bool GetChannelPaused(int i);

	void SetChannelVolume(int i, Uint8 volume);
	int GetChannelVolume(int i);
	void UnloadAllChunks();

private:
	//for fading (bugfix blocking)
	friend void fadeMiddle();
	Mix_Music* second;
	int inMilliseconds;

	static AudioMixer singleton;
	Mix_Music* music;
	std::map<std::string, Mix_Chunk*> chunks;
};