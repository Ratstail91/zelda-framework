# Zelda Framework

This is a hopefully-robust C++ game engine built on SDL2.

*While in the root scene, press space to enter the example scene (you may need to wait for the music to load), or tab to enter the editor scene. Press escape to return to the root scene.*

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

# Tools

`AudioMixer` is built on SDL2_mixer, and supports two types of sound: music and chunks. Music is designed for streaming audio directly from the file; the tool can only support one music file at a time. Chunks are separate pieces of sound effects which need to be loaded into memory in their entirety before use. `NodeAudioListener` and `NodeAudioSource` both utilize this tool.

`CameraBase` can be used for cameras, such as `CameraEditor`, and also supports updates in the same fashon as `NodeTransform`.

