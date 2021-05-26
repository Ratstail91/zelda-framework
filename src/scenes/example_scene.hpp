#pragma once

#include "base_scene.hpp"

#include "node_base.hpp"
#include "node_transform.hpp"
#include "node_actor.hpp"
#include "node_image.hpp"
#include "node_collider_box.hpp"
#include "node_audio_listener.hpp"
#include "node_audio_source.hpp"

#include "audio_mixer.hpp"
#include "camera_editor.hpp"

class ExampleScene : public BaseScene {
public:
	DECLARE_HIDING;
	DECLARE_BLOCKING;
	DECLARE_FREEZING;

	ExampleScene();
	virtual ~ExampleScene();

	void OnRenderFrame(SDL_Renderer* renderer) override;

private:
	//control hooks
	void OnEnter() override;
	void OnExit() override;

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

	//members
	NodeBase root;
	CameraEditor camera;

	//debugging
	int musicChannel;
};