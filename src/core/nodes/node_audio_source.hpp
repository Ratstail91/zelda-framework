#pragma once

#include "node_base.hpp"
#include "node_audio_listener.hpp"

#include <string>

class NodeAudioSource : public NodeBase {
public:
	//TODO: more wrapper methods
	NodeAudioSource(NodeAudioListener const* const, int);
	void PlayChunk(std::string const& key, int channel = -1, int loops = 0);
	void CalcVolume();

protected:
	NodeAudioListener const* listener = nullptr;
	int maxDistance = 0;
	int channel = -1;
};