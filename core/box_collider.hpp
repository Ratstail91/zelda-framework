#pragma once

#include <cmath>
#include <type_traits>

#include "vector2.hpp"

class BoxCollider {
public:
	Vector2 center;
	Vector2 halfSize;

	BoxCollider() = default;
	~BoxCollider() = default;
	BoxCollider& operator=(BoxCollider const&) = default;

	bool Intersect(BoxCollider const& other) const {
		//x-direction
		if (center.x - halfSize.x > other.center.x + other.halfSize.x) {
			return false;
		}

		if (center.x + halfSize.x < other.center.x - other.halfSize.x) {
			return false;
		}

		//y-direction
		if (center.y - halfSize.y > other.center.y + other.halfSize.y) {
			return false;
		}

		if (center.y + halfSize.y < other.center.y - other.halfSize.y) {
			return false;
		}

		return true;
	}

	//returns the new center of this bounding box (up to the calling context to set the actual position)
	Vector2 Snap(BoxCollider const& other) const {
		//do nothing if no intersection
		if (!Intersect(other)) {
			return Vector2(0.0, 0.0);
		}

		//snap to the closest position outside of other
		double jumpX = -(halfSize.x + other.halfSize.x -fabs(center.x - other.center.x));
		double jumpY = -(halfSize.y + other.halfSize.y -fabs(center.y - other.center.y));

		//floating ABS
		if (fabs(jumpX) < fabs(jumpY)) {
			return Vector2(jumpX, 0);
		} else {
			return Vector2(0, jumpY);
		}
	}

	//arithmetic operators
	BoxCollider operator+(Vector2 v) const {
		//adjust position
		BoxCollider box;
		box.center = center + v;
		box.halfSize = halfSize;
		return box;
	}

	//member templates (curry the above operators)
	template<typename T> BoxCollider operator+=(T t) { return *this = *this + t; }
};

//This is explicitly a POD
static_assert(std::is_pod<BoxCollider>::value, "BoxCollider is not a POD");
