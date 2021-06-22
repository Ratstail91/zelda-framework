#pragma once

#include "colliders.hpp"
#include "node_base.hpp"

class NodeColliderBox final : public NodeBase, public ColliderBox {
public:
	void SetBoundsToImageSibling();

	ColliderBox GetWorldColliderBox();

	void SnapCollideWorldBox(ColliderBox const& otherWorldBox);

private:
	using NodeBase::AddChild;
	using NodeBase::GetChild;
	using NodeBase::RemoveChild;
};