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
void EditorScene::OnFrameStart() {
	//
}

void EditorScene::OnUpdate() {
	//
}

void EditorScene::OnFrameEnd() {
	//
}

void EditorScene::OnRenderFrame(SDL_Renderer* renderer) {
	//
}

//input events
void EditorScene::OnMouseMotion(SDL_MouseMotionEvent const& event) {
	camera.MouseMotion(event);
}

void EditorScene::OnMouseButtonDown(SDL_MouseButtonEvent const& event) {
	camera.MouseButtonDown(event);
}

void EditorScene::OnMouseButtonUp(SDL_MouseButtonEvent const& event) {
	camera.MouseButtonUp(event);
}

void EditorScene::OnMouseWheel(SDL_MouseWheelEvent const& event) {
	camera.MouseWheel(event, GetRenderer());
}

void EditorScene::OnKeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			SetSceneSignal(SceneSignal::POP);
		break;
	}
}

void EditorScene::OnKeyUp(SDL_KeyboardEvent const& event) {
	//
}

void EditorScene::OnControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void EditorScene::OnControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void EditorScene::OnControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}