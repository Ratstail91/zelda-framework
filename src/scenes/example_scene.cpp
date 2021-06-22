#include "example_scene.hpp"

#include <future>
#include <iostream>
#include <vector>

ExampleScene::ExampleScene() {
	//wire up audio nodes
	NodeAudioListener* listener = new NodeAudioListener();

	//fill the tree
	root.AddChild(new Node()); //thing
	root.AddChild(new Node()); //dragon

	//ground
	root.GetChild(0)->AddChild(new NodeTransform());
	root.GetChild(0)->AddChild(new NodeImage(GetRenderer(), "rsc/thing.png"));
	root.GetChild(0)->AddChild(new NodeColliderBox());
	root.GetChild(0)->AddChild(new NodeAudioSource(listener, 1000)); // 0
	root.GetChild(0)->AddChild(new NodeAudioSource(listener, 1000)); // 1
	root.GetChild(0)->AddChild(new NodeAudioSource(listener, 1000)); // 2
	root.GetChild(0)->AddChild(new NodeAudioSource(listener, 1000)); // 3
	root.GetChild(0)->AddChild(new NodeAudioSource(listener, 1000)); // 4

	//dragon
	root.GetChild(1)->AddChild(new NodeTransform());
	root.GetChild(1)->AddChild(new NodeImage(GetRenderer(), "rsc/fairy dragon.png"));
	root.GetChild(1)->AddChild(new NodeColliderBox());
	root.GetChild(1)->AddChild(new NodeActor());
	root.GetChild(1)->AddChild(listener);

	//position the ground
	((NodeTransform*)(root.GetChild(0)->GetChild(0)))->GetPosition().y = 400;

	//correct the positions of the colliders
	std::list<NodeColliderBox*> colliderBoxes = root.GetDescendantsByType<NodeColliderBox>();
	for (NodeColliderBox* box : colliderBoxes) {
		box->SetBoundsToImageSibling();
	}

	//DEBUG: test the audio
	//using threads for a 2% speed increase
	auto threads = {
		std::async(std::launch::async, [](){ AudioMixer::GetSingleton().LoadChunk("01-base", "rsc/01_Base.ogg"); }),
		std::async(std::launch::async, [](){ AudioMixer::GetSingleton().LoadChunk("01-beat", "rsc/01_Beat.ogg"); }),
		std::async(std::launch::async, [](){ AudioMixer::GetSingleton().LoadChunk("01-lead", "rsc/01_Lead.ogg"); }),
		std::async(std::launch::async, [](){ AudioMixer::GetSingleton().LoadChunk("01-arp", "rsc/01_ARP.ogg"); }),
		std::async(std::launch::async, [](){ AudioMixer::GetSingleton().LoadChunk("01-seq", "rsc/01_SEQ.ogg"); })
	};

	for (auto& thread : threads) {
		thread.wait();
	}

	// AudioMixer::GetSingleton().LoadChunk("01-base", "rsc/01_Base.ogg");
	// AudioMixer::GetSingleton().LoadChunk("01-beat", "rsc/01_Beat.ogg");
	// AudioMixer::GetSingleton().LoadChunk("01-lead", "rsc/01_Lead.ogg");
	// AudioMixer::GetSingleton().LoadChunk("01-arp", "rsc/01_ARP.ogg");
	// AudioMixer::GetSingleton().LoadChunk("01-seq", "rsc/01_SEQ.ogg");

	auto sources = root.GetDescendantsByType<NodeAudioSource>();

	auto iter = sources.begin();
	(*(iter++))->PlayChunk("01-base", 0, -1);
//	(*(iter++))->PlayChunk("01-beat", 1, -1);
	(*(iter++))->PlayChunk("01-lead", 2, -1);
	(*(iter++))->PlayChunk("01-arp", 3, -1);
//	(*(iter++))->PlayChunk("01-seq", 4, -1);
}

ExampleScene::~ExampleScene() {
	AudioMixer::GetSingleton().StopAllChannels();

	removeDescendantsOfNode(&root);
}

//control hooks
void ExampleScene::OnEnter() {
	//
}

void ExampleScene::OnExit() {
	//
}

//frame phases
void ExampleScene::OnFrameStart() {
	//
}

void ExampleScene::OnUpdate() {
	Vector2 gravity = {0, 0.1};
	double friction = 0.01;

	auto actors = root.GetDescendantsByType<NodeActor>();

	for (auto actorPtr : actors) {
		actorPtr->Update(gravity, friction);
	}
}

void ExampleScene::OnFrameEnd() {
	//DEBUG: snap the dragon
	root.GetChild(1)->GetFirstChildByType<NodeColliderBox>()->SnapCollideWorldBox( root.GetChild(0)->GetFirstChildByType<NodeColliderBox>()->GetWorldColliderBox() );

	//cache listener positions
	auto listeners = root.GetDescendantsByType<NodeAudioListener>();

	for (auto listenerNode : listeners) {
		listenerNode->GetWorldPosition(); //cache the listener positions
	}

	//re-calc source volumes
	auto sources = root.GetDescendantsByType<NodeAudioSource>();

	for (auto sourceNode : sources) {
		sourceNode->CalcVolume(); //adjust the sources automatically based on the listener's position
	}
}

void ExampleScene::OnRenderFrame(SDL_Renderer* renderer) {
	auto images = root.GetDescendantsByType<NodeImage>();

	for (auto imageNode : images) {
		imageNode->DrawTo(renderer, camera.GetPosition().x, camera.GetPosition().y, camera.GetScale().x, camera.GetScale().y);
	}
}

//input events
void ExampleScene::OnMouseMotion(SDL_MouseMotionEvent const& event) {
	camera.MouseMotion(event);
}

void ExampleScene::OnMouseButtonDown(SDL_MouseButtonEvent const& event) {
	camera.MouseButtonDown(event);
}

void ExampleScene::OnMouseButtonUp(SDL_MouseButtonEvent const& event) {
	camera.MouseButtonUp(event);
}

void ExampleScene::OnMouseWheel(SDL_MouseWheelEvent const& event) {
	camera.MouseWheel(event);
}

void ExampleScene::OnKeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			SetSceneSignal(SceneSignal::POP);
		break;

		case SDLK_SPACE:
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion().y = -8;
		break;

		case SDLK_LEFT:
		case SDLK_a:
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion().x = -1;
		break;

		case SDLK_RIGHT:
		case SDLK_d:
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion().x = 1;
		break;

		case SDLK_m:
			//toggle music
			// if (AudioMixer::GetSingleton().GetMusicPaused()) {
			// 	AudioMixer::GetSingleton().UnpauseMusic();
			// } else {
			// 	AudioMixer::GetSingleton().PauseMusic();
			// }
		break;
	}
}

void ExampleScene::OnKeyUp(SDL_KeyboardEvent const& event) {
	//
}

void ExampleScene::OnControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	constexpr int epsilon = 1000;
	constexpr auto sign = [](auto x) -> int { return (x > 0) - (x < 0); };

	if (event.which == 0) {
		if (event.axis == SDL_CONTROLLER_AXIS_LEFTX && std::abs(event.value) > epsilon) {
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion().x = sign(event.value);
		}
	}
}

void ExampleScene::OnControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	if (event.which == 0) {
		if (event.button == SDL_CONTROLLER_BUTTON_A) {
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion().y = -8;
		}
	}
}

void ExampleScene::OnControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}