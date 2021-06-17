#pragma once

#include "collider_box.hpp"
#include "node_base.hpp"

class NodeColliderBox : public NodeBase, public ColliderBox {
public:
	void SetBoundsToImageSibling();

	bool Intersect(NodeColliderBox const& other);
	bool SnapCollide(NodeColliderBox const& other);
};