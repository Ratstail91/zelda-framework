#pragma once

#include "node_base.hpp"
#include "node_audio_listener.hpp"

#include <string>

class NodeAudioSource : public NodeBase {
public:
	NodeAudioSource(NodeAudioListener*, double);
	void PlayChunk(std::string key);
	void CalcVolume();

protected:
	NodeAudioListener* listener = nullptr;
	double maxDistance = 0;
	int channel = -1;
};