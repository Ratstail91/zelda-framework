#include "node_audio_listener.hpp"

#include "node_transform.hpp"
#include "node_collider_box.hpp"

Vector2 NodeAudioListener::GetWorldPosition() {
	return cachedPosition = parent->GetFirstChildByType<NodeTransform>()->GetWorldPosition() + parent->GetFirstChildByType<NodeColliderBox>()->center;
}

Vector2 NodeAudioListener::GetCachedPosition() const {
	return cachedPosition;
}