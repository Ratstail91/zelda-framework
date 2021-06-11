#pragma once

#include "node_base.hpp"
#include "node_audio_listener.hpp"

#include <string>

class NodeAudioSource : public NodeBase {
public:
	NodeAudioSource(NodeAudioListener const* const, double);
	void PlayChunk(std::string const& key);
	void CalcVolume();

protected:
	NodeAudioListener const* listener = nullptr;
	double maxDistance = 0;
	int channel = -1;
};