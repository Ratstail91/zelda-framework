#include "camera_editor.hpp"

void CameraEditor::MouseMotion(SDL_MouseMotionEvent const& event) {
	if (buttonDown) {
		position.x += event.xrel / scale.x;
		position.y += event.yrel / scale.y;
	}
}

void CameraEditor::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	if (event.button == SDL_BUTTON_RIGHT) {
		buttonDown = true;
	}
}

void CameraEditor::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	if (event.button == SDL_BUTTON_RIGHT) {
		buttonDown = false;
	}
}

void CameraEditor::MouseWheel(SDL_MouseWheelEvent const& event, SDL_Renderer* const renderer) {
	//scroll around the mouse cursor
	int x, y, w, h;
	SDL_GetMouseState(&x, &y);

	if (event.y > 0) {
		position.x -= x / scale.x;
		position.y -= y / scale.y;

		scale *= 1.1;
		ScaleBoundsCheck();

		position.x += x / scale.x;
		position.y += y / scale.y;
	}

	if (event.y < 0) {
		position.x -= x / scale.x;
		position.y -= y / scale.y;

		scale /= 1.1;
		ScaleBoundsCheck();

		position.x += x / scale.x;
		position.y += y / scale.y;
	}
}

void CameraEditor::ScaleBoundsCheck() {
	if (scale.x < 0.25) {
		scale.x = 0.25;
	}

	if (scale.y < 0.25) {
		scale.y = 0.25;
	}

	if (scale.x > 10.0) {
		scale.x = 10.0;
	}

	if (scale.y > 10.0) {
		scale.y = 10.0;
	}
}