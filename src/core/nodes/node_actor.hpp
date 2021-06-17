#pragma once

#include "node_base.hpp"

#include "vector2.hpp"

class NodeActor : public NodeBase {
public:
	void Update(Vector2 const& gravity = {0.0, 0.0}, double const friction = 0.0);
	void Rewind(Vector2 const& gravity = {0.0, 0.0}, double const friction = 0.0);
};