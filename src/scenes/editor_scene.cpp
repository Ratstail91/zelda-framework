#include "editor_scene.hpp"

EditorScene::EditorScene() {
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

EditorScene::~EditorScene() {
	deleteNode(&root);
}

//frame phases
void EditorScene::FrameStart() {
	//
}

void EditorScene::Update() {
	//
}

void EditorScene::FrameEnd() {
	//
}

void EditorScene::RenderFrame(SDL_Renderer* renderer) {
	//
}

//input events
void EditorScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	camera.MouseMotion(event);
}

void EditorScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	camera.MouseButtonDown(event);
}

void EditorScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	camera.MouseButtonUp(event);
}

void EditorScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	camera.MouseWheel(event, GetRenderer());
}

void EditorScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			SetSceneSignal(SceneSignal::POP);
		break;
	}
}

void EditorScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}

void EditorScene::ControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void EditorScene::ControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void EditorScene::ControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}