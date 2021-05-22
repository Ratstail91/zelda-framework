#include "node_actor.hpp"

#include "node_transform.hpp"

void NodeActor::Update(Vector2 gravity, double friction) {
	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();

	if (transform != nullptr) {
		*(transform->GetMotion()) += gravity;
		*(transform->GetMotion()) *= 1.0 - friction;
		*(transform->GetPosition()) += *(transform->GetMotion());
	}
}