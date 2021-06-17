#include "audio_mixer.hpp"

#include <sstream>
#include <stdexcept>

using namespace std::string_literals;

static void error(const std::string& str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

AudioMixer* AudioMixer::singleton = nullptr;

AudioMixer& AudioMixer::GetSingleton() {
	if (!singleton) {
		singleton = new AudioMixer();
	}
	return *singleton;
}

void AudioMixer::Init() {
	//Initialize SDL_mixer
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 8, 4096) != 0) {
		error("Failed to open audio"s);
	}

	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
		error("Failed to initialize OGG format"s);
	}
}

void AudioMixer::Quit() {
	Mix_Quit();
	Mix_CloseAudio();
}

void AudioMixer::LoadMusic(const std::string& fname) {
	UnloadMusic();

	music = Mix_LoadMUS(fname.c_str());

	if (music == nullptr) {
		error("Failed to load music file "s + fname);
	}
}

void AudioMixer::UnloadMusic() {
	if (music != nullptr) {
		Mix_FreeMusic(music);
		music = nullptr;
	}
}

void AudioMixer::PlayMusic() {
	if (Mix_PlayMusic(music, -1) != 0) {
		error("Failed to play music"s);
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

bool AudioMixer::GetMusicLoaded() {
	return music != nullptr;
}

bool AudioMixer::GetMusicPlaying() {
	return Mix_PlayingMusic();
}

bool AudioMixer::GetMusicPaused() {
	//is playing but paused
	return Mix_PlayingMusic() && Mix_PausedMusic();
}

void AudioMixer::SetMusicVolume(Uint8 volume) {
	Mix_VolumeMusic(volume);
}

int AudioMixer::GetMusicVolume() {
	return Mix_VolumeMusic(-1);
}

void AudioMixer::FadeMusicIn(int ms) {
	if (Mix_FadeInMusic(music, -1, ms) != 0) {
		error("Failed to fade in music"s);
	}
}

void AudioMixer::FadeMusicOut(int ms) {
	Mix_FadeOutMusic(ms);
}

//friend function
void fadeMiddle() {
	AudioMixer::GetSingleton().UnloadMusic();
	Mix_HookMusicFinished(nullptr);

	AudioMixer::GetSingleton().music = AudioMixer::GetSingleton().second;
	AudioMixer::GetSingleton().FadeMusicIn(AudioMixer::GetSingleton().inMilliseconds);
}

void AudioMixer::FadeMusicTo(const std::string& fname, int outMs, int inMs) {
	//load second file
	second = Mix_LoadMUS(fname.c_str());

	if (second == nullptr) {
		error("Failed to load music file "s + fname);
	}

	Mix_FadeOutMusic(outMs);

	Mix_HookMusicFinished(fadeMiddle);

	//for use in fadeMiddle()
	inMilliseconds = inMs;
}

void AudioMixer::LoadChunk(const std::string& key, const std::string& fname) {
	if (chunks.find(key) != chunks.end()) {
		return; //already loaded
	}

	Mix_Chunk* chunk = Mix_LoadWAV(fname.c_str());

	if (chunk == nullptr) {
		error("Failed to load a chunk file "s + fname);
	}

	std::lock_guard<std::mutex> lock(loadChunkMutex);
	chunks[key] = chunk;
}

void AudioMixer::UnloadChunk(const std::string& key) {
	auto it = chunks.find(key);

	if (it == chunks.end()) {
		return; //already unloaded
	}

	Mix_FreeChunk(it->second);

	chunks.erase(it);
}

int AudioMixer::PlayChunk(const std::string& key, int channel, int loops) {
	auto it = chunks.find(key);

	if (it == chunks.end()) {
		error("Chunk not loaded: "s + key);
	}

	int result = Mix_PlayChannel(channel, it->second, loops);

	if (result == -1) {
		error("Could not play chunk "s + key);
	}

	return result;
}

void AudioMixer::PauseChannel(int i) {
	Mix_Pause(i);
}

void AudioMixer::UnpauseChannel(int i) {
	Mix_Resume(i);
}

void AudioMixer::StopChannel(int i) {
	Mix_HaltChannel(i);
}

void AudioMixer::StopAllChannels() {
	const int total = Mix_AllocateChannels(-1);

	for (int i = 0; i < total; i++) {
		StopChannel(i);
	}
}

bool AudioMixer::GetChunkLoaded(const std::string& key) {
	return chunks.find(key) != chunks.end();
}

bool AudioMixer::GetChannelPlaying(int i) {
	return Mix_Playing(i);
}

bool AudioMixer::GetChannelPaused(int i) {
	return Mix_Playing(i) && Mix_Paused(i);
}

void AudioMixer::SetChannelVolume(int i, Uint8 volume) {
	Mix_Volume(i, volume);
}

int AudioMixer::GetChannelVolume(int i) {
	return Mix_Volume(i, -1);
}

void AudioMixer::UnloadAllChunks() {
	for (auto it : chunks) {
		Mix_FreeChunk(it.second);
	}

	chunks.clear();
}