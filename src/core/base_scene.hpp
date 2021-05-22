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

	virtual void RenderFrame(SDL_Renderer*);
	static void SetRenderer(SDL_Renderer*);

	void SetSceneSignal(SceneSignal);
	SceneSignal GetSceneSignal();

	//hiding & blocking signals
	virtual bool GetHiding() const { return false; }
	virtual bool GetBlocking() const { return false; }
	virtual bool GetFreezing() const { return false; }

	//frame phases
	virtual void FrameStart();
	virtual void Update();
	virtual void FrameEnd();

	//input events
	virtual void QuitEvent();
	virtual void MouseMotion(SDL_MouseMotionEvent const& event);
	virtual void MouseButtonDown(SDL_MouseButtonEvent const& event);
	virtual void MouseButtonUp(SDL_MouseButtonEvent const& event);
	virtual void MouseWheel(SDL_MouseWheelEvent const& event);
	virtual void KeyDown(SDL_KeyboardEvent const& event);
	virtual void KeyUp(SDL_KeyboardEvent const& event);
	//TODO: joystick events
	virtual void ControllerAxisMotion(SDL_ControllerAxisEvent const& event);
	virtual void ControllerButtonDown(SDL_ControllerButtonEvent const& event);
	virtual void ControllerButtonUp(SDL_ControllerButtonEvent const& event);

protected:
	//control
	static SDL_Renderer* GetRenderer();

private:
	static SDL_Renderer* rendererHandle;
	SceneSignal sceneSignal = SceneSignal::CONTINUE;
};