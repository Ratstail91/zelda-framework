#pragma once

enum SceneSignal {
	//reserved members for internal use
	POP = -2,
	QUIT = -1,
	CONTINUE = 0,
	FIRST = 1,

	//custom scenes
	ROOT_SCENE,
	EXAMPLE_SCENE,
	EDITOR_SCENE,
	PONG_SCENE,
};