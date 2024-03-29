#include "node_audio_source.hpp"

#include "audio_mixer.hpp"
#include "node_transform.hpp"
#include "node_collider_box.hpp"

NodeAudioSource::NodeAudioSource(NodeAudioListener const* const ptr, int dist) {
	listener = ptr;
	maxDistance = dist;
}

void NodeAudioSource::PlayChunk(std::string const& key, int ch, int loops) {
	channel = AudioMixer::GetSingleton().PlayChunk(key, ch, loops);
	CalcVolume();
}

void NodeAudioSource::CalcVolume() {
	if (channel < 0) {
		return;
	}

	//can't really cache this object's position, this function is only called once a frame
	Vector2 me = parent->GetFirstChildByType<NodeTransform>()->GetWorldPosition() + parent->GetFirstChildByType<NodeColliderBox>()->center;
	Vector2 you = listener->GetCachedPosition();

	double distance = (me - you).Length();

	if (distance >= maxDistance) {
		AudioMixer::GetSingleton().SetChannelVolume(channel, 0);
		return;
	}

	double ratio = distance / maxDistance;

	AudioMixer::GetSingleton().SetChannelVolume(channel, 128 - ratio * 128);
}
