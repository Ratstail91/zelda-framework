#include "node_image.hpp"

#include "vector2.hpp"
#include "node_transform.hpp"

void NodeImage::DrawTo(SDL_Renderer* const renderer, Sint16 x, Sint16 y, double scaleX, double scaleY) {
	NodeTransform* transform = parent->GetFirstChildByType<NodeTransform>();

	Vector2 pos = {0, 0};
	Vector2 scl = {1.0, 1.0};

	if (transform != nullptr) {
		pos = transform->GetWorldPosition();
		scl = transform->GetWorldScale();
	}

	Image::DrawTo(renderer, x + pos.x, y + pos.y, scaleX * scl.x, scaleY * scl.y);
}