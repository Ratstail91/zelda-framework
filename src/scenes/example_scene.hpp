#pragma once

#include "base_scene.hpp"

#include "node_base.hpp"
#include "node_image.hpp"
#include "node_box_collider.hpp"

class ExampleScene : public BaseScene {
public:
	DECLARE_BLOCKING;

	ExampleScene();
	virtual ~ExampleScene();

	void RenderFrame(SDL_Renderer* renderer) override;

private:
	//frame phases
	void FrameStart() override;
	void Update() override;
	void FrameEnd() override;

	//input events
	void MouseMotion(SDL_MouseMotionEvent const& event) override;
	void MouseButtonDown(SDL_MouseButtonEvent const& event) override;
	void MouseButtonUp(SDL_MouseButtonEvent const& event) override;
	void MouseWheel(SDL_MouseWheelEvent const& event) override;
	void KeyDown(SDL_KeyboardEvent const& event) override;
	void KeyUp(SDL_KeyboardEvent const& event) override;
	void ControllerAxisMotion(SDL_ControllerAxisEvent const& event) override;
	void ControllerButtonDown(SDL_ControllerButtonEvent const& event) override;
	void ControllerButtonUp(SDL_ControllerButtonEvent const& event) override;

	//debugging
	NodeBase root;
};