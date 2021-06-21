#pragma once

#include <SDL2/SDL_mixer.h>

#include <future>
#include <mutex>
#include <string>
#include <map>

class AudioMixer {
public:
	//singleton pattern
	static AudioMixer& GetSingleton();

	//setup
	void Init();
	void Quit();

	//music controls
	void LoadMusic(const std::string&);
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
	void FadeMusicTo(const std::string&, int outMs, int inMs);

	//chunk controls
	void LoadChunk(const std::string& key, const std::string& fname);
	void UnloadChunk(const std::string& key);
	int PlayChunk(const std::string& key, int channel = -1, int loops = 0);
	void PauseChannel(int i);
	void UnpauseChannel(int i);
	void StopChannel(int i);
	void StopAllChannels();

	bool GetChunkLoaded(const std::string& key);
	bool GetChannelPlaying(int i);
	bool GetChannelPaused(int i);

	void SetChannelVolume(int i, Uint8 volume);
	int GetChannelVolume(int i);
	void UnloadAllChunks();

private:
	//for fading (bugfix blocking)
	friend void fadeMiddle();
	Mix_Music* second = nullptr;
	int inMilliseconds;

	//singleton stuff
	static AudioMixer* singleton;
	AudioMixer() = default;
	~AudioMixer() = default;

	//containers
	Mix_Music* music = nullptr;
	std::map<const std::string, Mix_Chunk*> chunks;

	//async stuff
	std::mutex loadChunkMutex;
};