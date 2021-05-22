#pragma once

#include "node_base.hpp"

#include "vector2.hpp"

class NodeActor : public NodeBase {
public:
	void Update(Vector2 gravity = {0.0, 0.0}, double friction = 0.0);
};