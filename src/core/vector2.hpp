#pragma once

#include <type_traits>
#include <stdexcept>
#include <cmath>

class Vector2 {
public:
	double x, y;

	Vector2() = default;
	Vector2(double i, double j): x(i), y(j) {};
	~Vector2() = default;
	Vector2& operator=(Vector2 const&) = default;

	double Length() const {
		return sqrt(x*x+y*y);
	}
	double SquaredLength() const {
		return x*x+y*y;
	}
	void Normalize() {
		double l = Length();
		if (l == 0)
			throw(std::domain_error("Divide by zero"));
		x /= l;
		y /= l;
	}

	//Arithmetic operators
	Vector2 operator+(Vector2 const& v) const {
		Vector2 ret;
		ret.x = x + v.x;
		ret.y = y + v.y;
		return ret;
	}
	Vector2 operator-(Vector2 const& v) const {
		Vector2 ret;
		ret.x = x - v.x;
		ret.y = y - v.y;
		return ret;
	}
	Vector2 operator*(Vector2 const& v) const {
		Vector2 ret;
		ret.x = x * v.x;
		ret.y = y * v.y;
		return ret;
	}
	Vector2 operator*(double const& d) const {
		Vector2 ret;
		ret.x = x * d;
		ret.y = y * d;
		return ret;
	}

	Vector2 operator/(Vector2 const& v) const {
		if (!v.x || !v.y)
			throw(std::domain_error("Divide by zero"));
		Vector2 ret;
		ret.x = x / v.x;
		ret.y = y / v.y;
		return ret;
	}
	Vector2 operator/(double const& d) const {
		if (!d)
			throw(std::domain_error("Divide by zero"));
		Vector2 ret;
		ret.x = x / d;
		ret.y = y / d;
		return ret;
	}

	//unary operators
	Vector2 operator-() const { return {-x, -y}; }

	//comparison operators
	bool operator==(Vector2 const& v) const { return (x == v.x && y == v.y); }
	bool operator!=(Vector2 const& v) const { return (x != v.x || y != v.y); }

	//member templates (curry the above operators)
	template<typename T> Vector2 operator+=(T t) { return *this = *this + t; }
	template<typename T> Vector2 operator-=(T t) { return *this = *this - t; }
	template<typename T> Vector2 operator*=(T t) { return *this = *this * t; }
	template<typename T> Vector2 operator/=(T t) { return *this = *this / t; }
	template<typename T> bool operator==(T t) { return (x == t && y == t); }
	template<typename T> bool operator!=(T t) { return (x != t || y != t); }
};

//This is explicitly a POD
static_assert(std::is_pod<Vector2>::value, "Vector2 is not a POD");
