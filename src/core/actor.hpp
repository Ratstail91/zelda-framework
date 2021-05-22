#pragma once

#include "image.hpp"
#include "vector2.hpp"
#include "box_collider.hpp"

#include "SDL2/SDL.h"

#include <string>

//DOCS: an actor is anything that appears on screen
class Actor {
public:
	Actor();
	virtual ~Actor();

	//graphics
	void LoadSprite(SDL_Renderer* const renderer, std::string fname);
	void DrawTo(SDL_Renderer* const renderer, Vector2 camera);

	//simulation
	void Update(Vector2 gravity = {0, 0}, double friction = 0.0);
	bool Intersect(Actor const& other);
	void SnapCollide(Actor const& other);

	//accessors
	Image* GetImage() { return &image; }
	Vector2* GetPosition() { return &position; }
	Vector2* GetMotion() { return &motion; }
	BoxCollider* GetCollider() { return &collider; }

protected:
	Image image;
	Vector2 position = {0, 0};
	Vector2 motion = {0, 0};
	BoxCollider collider = {{0, 0}, {0, 0}};
};