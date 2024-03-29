# Zelda Framework

This is a hopefully-robust C++ game engine built on SDL2.

TODO: Document the API

# Building Instructions

* Clone the repo and it's submodules
* Run `make` in the root directory
* Run `out\out` in the root directory to execute the program

This project has the following dependencies.

* [SDL2](https://www.libsdl.org/download-2.0.php)
* [SDL2_image](https://www.libsdl.org/projects/SDL_image/)
* [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/)

This project includes several external repositories as git submodules under `src/vendor`, and will not build without them.

*While in the root scene, press space to enter the example scene (you may need to wait for the music to load), or tab to enter the editor scene. Press escape to return to the root scene. You can also press 1 for a game of pong.*

# Scene System

The core of this engine consists of the scene system, which creates a "stack" of scenes placed one on top of another, on top of another. Scenes are rendered from back-to-front, and input is interpreted from front-to-back.

However, if a scene is marked with `DECLARE_HIDING`, then all scenes below it do not render; if a scene is marked with `DECLARE_BLOCKING`, then all scenes below it do not receive input from the player; if a scene is marked with `DECLARE_FREEZING`, then all scenes below it are not simulated (`OnFrameStart()`, `OnUpdate()` and `OnFrameEnd()` are skipped).

All scenes must inherit from `BaseScene` to be usable in the `Application` class. That scene must also be added to `src/scene_signal.hpp` and `src/scene_signal.cpp`.

# Node System

The entities in the 2D game world must exist in the node system by inheriting from `NodeBase`. `NodeBase` gives some utility functions for finding nodes of a specific type: `GetChildrenByType<T>()` will return all children of a node of type `T`, and all of their children and their children. However, `GetFirstChildByType<T>()` will return the first immediate child of type `T`, returning null if it's not found.

Many node types have dependancies among their siblings, i.e. they don't work correctly unless another node of a specific type exists. These dependancies are:

```
NodeImage:       NodeTransform
NodeColliderBox: NodeTransform, NodeImage
NodeAudioSource: NodeTransform, NodeColliderBox (and NodeImage)
NodeListener:    NodeTransform, NodeColliderBox (and NodeImage)
NodeActor:       NodeTransform
```

# Other Tools

`AudioMixer` is built on SDL2_mixer, and supports two types of sound: music and chunks. Music is designed for streaming audio directly from the file; the tool can only support one music file at a time. Chunks are separate pieces of sound effects which need to be loaded into memory in their entirety before use. `NodeAudioListener` and `NodeAudioSource` both utilize this tool.

`CameraBase` can be used for cameras, such as `CameraEditor`, and also supports updates in the same fashon as `NodeTransform`.

# Planned and Current Games

I'm planning on implementing some simple games to push the feature set forward. These games can be accessed by pressing the following numbers (or unimplemented games are marked with a *):

1. Pong  
2. *Snake  
3. *Bullet hell  
4. *Boulderdash  
5. *Tetris  
6. *Panel De Pon  
7. *Tower Blocks  

# Feedback I'll Work On Soon

...

