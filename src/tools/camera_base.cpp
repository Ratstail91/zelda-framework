#include "camera_base.hpp"

void CameraBase::Update(Vector2 gravity, double friction) {
	motion += gravity;
	motion *= 1.0 - friction;
	position += motion * scale;
}