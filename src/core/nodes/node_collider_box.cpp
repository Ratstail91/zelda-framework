#include "node_collider_box.hpp"

#include "node_transform.hpp"
#include "node_image.hpp"

void NodeColliderBox::SetBoundsToImageSibling() {
	//check if there is an image on this node already
	NodeImage* image = parent->GetFirstChildByType<NodeImage>();

	if (image != nullptr) {
		//automatically set up the bounds
		halfSize.x = image->GetClipW() / 2.0;
		halfSize.y = image->GetClipH() / 2.0;
		center = halfSize;
	}
}

bool NodeColliderBox::Intersect(NodeColliderBox const& other) {
	//get the two boxes
	ColliderBox me = *this;
	ColliderBox you = (ColliderBox)other;

	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();
	NodeTransform* otherTransform = other.GetParent()->GetFirstChildByType<NodeTransform>();

	if (transform != nullptr) {
		me += *(transform->GetPosition());
	}

	if (otherTransform != nullptr) {
		you += *(otherTransform->GetPosition());
	}

	//do we intersect?
	return me.Intersect(you);
}

void NodeColliderBox::SnapCollide(NodeColliderBox const& other) {
	//get the two boxes
	ColliderBox me = *this;
	ColliderBox you = (ColliderBox)other;

	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();
	NodeTransform* otherTransform = other.GetParent()->GetFirstChildByType<NodeTransform>();

	if (transform != nullptr) {
		me += *(transform->GetPosition());
	}

	if (otherTransform != nullptr) {
		you += *(otherTransform->GetPosition());
	}

	//jump to the correct position
	Vector2 jump = me.Snap(you);

	if (jump != 0.0) {
		*(transform->GetPosition()) += jump;
	}

	if (jump.x != 0.0) {
		transform->GetMotion()->x = 0;
	}

	if (jump.y != 0.0) {
		transform->GetMotion()->y = 0;
	}
}
