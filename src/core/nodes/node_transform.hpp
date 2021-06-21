#pragma once

#include "node_base.hpp"
#include "vector2.hpp"

class NodeTransform final : public NodeBase {
public:
	Vector2& GetPosition() { return position; }
	Vector2& GetMotion() { return motion; }
	Vector2& GetScale() { return scale; }

	Vector2 GetWorldPosition();
	Vector2 GetWorldMotion();
	Vector2 GetWorldScale();

private:
	Vector2 position = {0, 0};
	Vector2 motion = {0, 0};
	Vector2 scale = {1.0, 1.0};

	using NodeBase::AddChild;
	using NodeBase::GetChild;
	using NodeBase::RemoveChild;
};