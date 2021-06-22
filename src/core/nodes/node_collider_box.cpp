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

ColliderBox NodeColliderBox::GetWorldColliderBox() {
	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();

	if (transform != nullptr) {
		//adjust self based on transform's position
		return *this + transform->GetWorldPosition();
	}
	return *this;
}

void NodeColliderBox::SnapCollideWorldBox(ColliderBox const& otherWorldBox) {
	ColliderBox thisWorldBox = GetWorldColliderBox();

	//do nothing if no intersection
	if (!Intersect(thisWorldBox, otherWorldBox)) {
		return;
	}

	//snap to the closest position outside of other
	double jumpX = thisWorldBox.halfSize.x + otherWorldBox.halfSize.x - fabs(thisWorldBox.center.x - otherWorldBox.center.x);
	double jumpY = thisWorldBox.halfSize.y + otherWorldBox.halfSize.y - fabs(thisWorldBox.center.y - otherWorldBox.center.y);

	Vector2 jump;

	//floating ABS
	if (fabs(jumpX) < fabs(jumpY)) {
		//multiply by direction
		jump = Vector2(jumpX, 0) * (thisWorldBox.center.x > otherWorldBox.center.x ? 1 : -1);
	} else {
		//multiply by direction
		jump = Vector2(0, jumpY) * (thisWorldBox.center.y > otherWorldBox.center.y ? 1 : -1);
	}

	//get the transform and actually jump
	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();

	if (transform != nullptr) {
		transform->GetPosition() += jump;

		//stop motion in that direction
		if (jump.x != 0.0) {
			transform->GetMotion().x = 0;
		}

		if (jump.y != 0.0) {
			transform->GetMotion().y = 0;
		}
	}
}
