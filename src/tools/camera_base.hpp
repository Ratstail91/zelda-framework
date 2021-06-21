#pragma once

#include "vector2.hpp"

class CameraBase {
public:
	Vector2 GetPosition() { return position; }
	Vector2 GetMotion() { return motion; }
	Vector2 GetScale() { return scale; }

	void Update(Vector2 gravity = {0.0, 0.0}, double friction = 0.0);

protected:
	Vector2 position = {0, 0};
	Vector2 motion = {0, 0};
	Vector2 scale = {1.0, 1.0};
};