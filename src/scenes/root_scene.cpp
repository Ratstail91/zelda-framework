#include "root_scene.hpp"

RootScene::RootScene() {
	//
}

static void deleteNode(NodeBase* root) {
	while (true) {
		NodeBase* child = root->GetChild(0);

		if (child == nullptr) {
			break;
		}

		deleteNode(child);

		root->RemoveChild(0);
		delete child;
	}
}

RootScene::~RootScene() {
	deleteNode(&root);
}

//frame phases
void RootScene::FrameStart() {
	//
}

void RootScene::Update() {
	//
}

void RootScene::FrameEnd() {
	//
}

void RootScene::RenderFrame(SDL_Renderer* renderer) {
	//
}

//input events
void RootScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void RootScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void RootScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void RootScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void RootScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			SetSceneSignal(SceneSignal::POP);
		break;

		case SDLK_SPACE:
			SetSceneSignal(SceneSignal::EXAMPLE_SCENE);
		break;

		case SDLK_TAB:
			SetSceneSignal(SceneSignal::EDITOR_SCENE);
		break;
	}
}

void RootScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}

void RootScene::ControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void RootScene::ControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void RootScene::ControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}