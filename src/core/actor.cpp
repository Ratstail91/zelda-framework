#include "actor.hpp"

Actor::Actor() {
	//
}

Actor::~Actor() {
	//
}

void Actor::LoadSprite(SDL_Renderer* const renderer, std::string fname) {
	image.Load(renderer, fname);

	position = {0, 0};

	collider.halfSize.x = image.GetClipW() / 2.0;
	collider.halfSize.y = image.GetClipH() / 2.0;
	collider.center = collider.halfSize; //place the center of the box collider, in the actual center
}

void Actor::DrawTo(SDL_Renderer* const renderer, Vector2 camera) {
	Vector2 screenPosition = position - camera;

	image.DrawTo(renderer, screenPosition.x, screenPosition.y);
}

void Actor::Update(Vector2 gravity, double friction) {
	motion += gravity;
	motion *= 1.0 - friction;
	position += motion;
}

bool Actor::Intersect(Actor const& other) {
	ColliderBox me = collider + position;
	ColliderBox you = other.collider + other.position;

	return me.Intersect(you);
}

void Actor::SnapCollide(Actor const& other) {
	ColliderBox me = collider + position;
	ColliderBox you = other.collider + other.position;

	Vector2 jump = me.Snap(you);

	if (jump != 0.0) {
		position += jump;
	}

	if (jump.x != 0.0) {
		motion.x = 0;
	}

	if (jump.y != 0.0) {
		motion.y = 0;
	}
}