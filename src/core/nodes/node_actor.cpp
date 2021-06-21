#include "node_actor.hpp"

#include "node_transform.hpp"

void NodeActor::Update(Vector2 const& gravity, double const friction) {
	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();

	if (transform != nullptr) {
		transform->GetMotion() += gravity;
		transform->GetMotion() *= 1.0 - friction;
		transform->GetPosition() += transform->GetMotion();
	}
}

void NodeActor::Rewind(Vector2 const& gravity, double const friction) {
	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();

	if (transform != nullptr) {
		transform->GetPosition() -= transform->GetMotion();
		transform->GetMotion() /= 1.0 - friction;
		transform->GetMotion() -= gravity;
	}
}