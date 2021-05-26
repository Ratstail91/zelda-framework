#include "editor_scene.hpp"

#include "imgui_impl_sdl.h"

EditorScene::EditorScene() {
	//
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
	char buf[128];
	float f;

	ImGui::Begin("title");
	ImGui::Text("Hello, world %d", 123);
	if (ImGui::Button("Save"))
		;
	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	ImGui::End();
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