#pragma once

#include "camera_base.hpp"

#include <SDL2/SDL.h>

class CameraEditor : public CameraBase {
public:
	//input events
	void MouseMotion(SDL_MouseMotionEvent const& event);
	void MouseButtonDown(SDL_MouseButtonEvent const& event);
	void MouseButtonUp(SDL_MouseButtonEvent const& event);
	void MouseWheel(SDL_MouseWheelEvent const& event);

private:
	void ScaleBoundsCheck();
	bool buttonDown = false;
};