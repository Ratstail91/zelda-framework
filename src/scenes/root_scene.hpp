#pragma once

#include "base_scene.hpp"

#include "node.hpp"

class RootScene : public BaseScene {
public:
	DECLARE_HIDING;
	DECLARE_BLOCKING;
	DECLARE_FREEZING;

	RootScene();
	virtual ~RootScene();

	void OnRenderFrame(SDL_Renderer* renderer) override;

private:
	//frame phases
	void OnFrameStart() override;
	void OnUpdate() override;
	void OnFrameEnd() override;

	//input events
	void OnMouseMotion(SDL_MouseMotionEvent const& event) override;
	void OnMouseButtonDown(SDL_MouseButtonEvent const& event) override;
	void OnMouseButtonUp(SDL_MouseButtonEvent const& event) override;
	void OnMouseWheel(SDL_MouseWheelEvent const& event) override;
	void OnKeyDown(SDL_KeyboardEvent const& event) override;
	void OnKeyUp(SDL_KeyboardEvent const& event) override;
	void OnControllerAxisMotion(SDL_ControllerAxisEvent const& event) override;
	void OnControllerButtonDown(SDL_ControllerButtonEvent const& event) override;
	void OnControllerButtonUp(SDL_ControllerButtonEvent const& event) override;

	//debugging
	Node root;
};