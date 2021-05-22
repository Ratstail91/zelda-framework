#include "example_scene.hpp"

#include <iostream>

ExampleScene::ExampleScene() {
	//EMPTY

	dragon.LoadSprite(GetRenderer(), "rsc/fairy dragon.png");
	wall.LoadSprite(GetRenderer(), "rsc/thing.png");

	//debug
	wall.GetPosition()->x = 200;
	wall.GetPosition()->y = 400;
}

ExampleScene::~ExampleScene() {
	//EMPTY
}

//frame phases
void ExampleScene::FrameStart() {
	//
}

void ExampleScene::Update() {
	Vector2 gravity = {0, 0.1};
	double friction = 0.01;

	dragon.Update(gravity, friction);
	wall.Update({0, 0}, friction);
}

void ExampleScene::FrameEnd() {
	dragon.SnapCollide(wall);
}

void ExampleScene::RenderFrame(SDL_Renderer* renderer) {
	Vector2 camera = {0, 0};

	dragon.DrawTo(renderer, camera);
	wall.DrawTo(renderer, camera);
}

//input events
void ExampleScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void ExampleScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void ExampleScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
		break;

		case SDLK_SPACE:
			dragon.GetMotion()->y = -8;
		break;

		case SDLK_LEFT:
		case SDLK_a:
			dragon.GetMotion()->x = -1;
		break;

		case SDLK_RIGHT:
		case SDLK_d:
			dragon.GetMotion()->x = 1;
		break;
	}
}

void ExampleScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}

void ExampleScene::ControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void ExampleScene::ControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void ExampleScene::ControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}