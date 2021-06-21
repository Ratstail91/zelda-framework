#include "node_transform.hpp"

Vector2 NodeTransform::GetWorldPosition() {
	Vector2 w = {0, 0};

	//ancestor transform
	NodeBase* p = parent;

	while (p != nullptr) {
		NodeTransform* t = nullptr;

		while (p != nullptr && (t = p->GetFirstChildByType<NodeTransform>()) == nullptr) {
			p = p->GetParent();
		}

		if (t != nullptr) {
			w += t->GetPosition();
			p = p->GetParent();
		}
	}

	return w;
}

Vector2 NodeTransform::GetWorldMotion() {
	Vector2 w = {0, 0};

	//ancestor transform
	NodeBase* p = parent;

	while (p != nullptr) {
		NodeTransform* t = nullptr;

		while (p != nullptr && (t = p->GetFirstChildByType<NodeTransform>()) == nullptr) {
			p = p->GetParent();
		}

		if (t != nullptr) {
			w += t->GetMotion();
			p = p->GetParent();
		}
	}

	return w;
}

Vector2 NodeTransform::GetWorldScale() {
	Vector2 w = {1.0, 1.0};

	//ancestor transform
	NodeBase* p = parent;

	while (p != nullptr) {
		NodeTransform* t = nullptr;

		while (p != nullptr && (t = p->GetFirstChildByType<NodeTransform>()) == nullptr) {
			p = p->GetParent();
		}

		if (t != nullptr) {
			w *= t->GetScale();
			p = p->GetParent();
		}
	}

	return w;
}