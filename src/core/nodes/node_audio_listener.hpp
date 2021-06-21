#pragma once

#include "node_base.hpp"

#include "vector2.hpp"

class NodeAudioListener final : public NodeBase {
public:
	Vector2 GetWorldPosition();
	Vector2 GetCachedPosition() const;

private:
	Vector2 cachedPosition = {0, 0}; //otherwise, the audio sources would run up the node tree every single time this was accessed

	using NodeBase::AddChild;
	using NodeBase::GetChild;
	using NodeBase::RemoveChild;
};