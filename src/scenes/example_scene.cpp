#include "example_scene.hpp"

#include <iostream>

ExampleScene::ExampleScene() {
	//EMPTY

	root.AddChild(new NodeImage(GetRenderer(), "rsc/thing.png"));
	NodeBase* child = root.GetChild(0);

	child->AddChild(new NodeImage(GetRenderer(), "rsc/fairy dragon.png"));
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

	//TODO
}

void ExampleScene::FrameEnd() {
	//
}

void ExampleScene::RenderFrame(SDL_Renderer* renderer) {
	Vector2 camera = {0, 0};

	auto images = root.GetChildrenByType<NodeImage>();

	for (auto it = images.begin(); it != images.end(); it++) {
		(*it)->DrawTo(renderer, camera.x, camera.y);
	}
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
			//
		break;

		case SDLK_LEFT:
		case SDLK_a:
			//
		break;

		case SDLK_RIGHT:
		case SDLK_d:
			//
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