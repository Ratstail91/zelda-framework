#pragma once

#include "scene_signal.hpp"

#include "SDL2/SDL.h"

//hide lower scenes
#define DECLARE_HIDING bool GetHiding() const override { return true; }

//stop input to lower scenes
#define DECLARE_BLOCKING bool GetBlocking() const override { return true; }

//freeze simulation in lower scenes
#define DECLARE_FREEZING bool GetFreezing() const override { return true; }

class BaseScene {
public:
	BaseScene();
	virtual ~BaseScene();

	virtual void OnRenderFrame(SDL_Renderer*);
	static void SetRenderer(SDL_Renderer*);

	void SetSceneSignal(SceneSignal);
	SceneSignal GetSceneSignal();

	//control hooks
	virtual void OnEnter();
	virtual void OnExit();

	//hiding & blocking signals
	virtual bool GetHiding() const { return false; }
	virtual bool GetBlocking() const { return false; }
	virtual bool GetFreezing() const { return false; }

	//frame phases
	virtual void OnFrameStart();
	virtual void OnUpdate();
	virtual void OnFrameEnd();

	//input events
	virtual void OnQuitEvent();
	virtual void OnMouseMotion(SDL_MouseMotionEvent const& event);
	virtual void OnMouseButtonDown(SDL_MouseButtonEvent const& event);
	virtual void OnMouseButtonUp(SDL_MouseButtonEvent const& event);
	virtual void OnMouseWheel(SDL_MouseWheelEvent const& event);
	virtual void OnKeyDown(SDL_KeyboardEvent const& event);
	virtual void OnKeyUp(SDL_KeyboardEvent const& event);
	//TODO: joystick events
	virtual void OnControllerAxisMotion(SDL_ControllerAxisEvent const& event);
	virtual void OnControllerButtonDown(SDL_ControllerButtonEvent const& event);
	virtual void OnControllerButtonUp(SDL_ControllerButtonEvent const& event);

protected:
	//control
	static SDL_Renderer* GetRenderer();

private:
	static SDL_Renderer* rendererHandle;
	SceneSignal sceneSignal = SceneSignal::CONTINUE;
};