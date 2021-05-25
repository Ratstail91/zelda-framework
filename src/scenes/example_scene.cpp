#include "example_scene.hpp"

#include <sstream>
#include <stdexcept>

static void error(std::string str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

ExampleScene::ExampleScene() {
	//fill the tree
	root.AddChild(new NodeBase());
	root.AddChild(new NodeBase());

	//ground
	root.GetChild(0)->AddChild(new NodeTransform());
	root.GetChild(0)->AddChild(new NodeImage(GetRenderer(), "rsc/thing.png"));
	root.GetChild(0)->AddChild(new NodeColliderBox());

	//dragon
	root.GetChild(1)->AddChild(new NodeTransform());
	root.GetChild(1)->AddChild(new NodeImage(GetRenderer(), "rsc/fairy dragon.png"));
	root.GetChild(1)->AddChild(new NodeColliderBox());
	root.GetChild(1)->AddChild(new NodeActor());

	((NodeTransform*)(root.GetChild(0)->GetChild(0)))->GetPosition()->y = 400;

	std::list<NodeColliderBox*> colliderBoxes = root.GetChildrenByType<NodeColliderBox>();

	for (NodeColliderBox* box : colliderBoxes) {
		box->SetBoundsToImageSibling();
	}
}

static void deleteNode(NodeBase* root) {
	while (true) {
		NodeBase* child = root->GetChild(0);

		if (child == nullptr) {
			break;
		}

		deleteNode(child);

		root->RemoveChild(0);
		delete child;
	}
}

ExampleScene::~ExampleScene() {
	deleteNode(&root);
}

//control hooks
void ExampleScene::OnEnter() {
	//AudioMixer::GetSingleton().LoadChunk("music", "rsc/EngineTest.ogg");
	AudioMixer::GetSingleton().LoadMusic("rsc/EngineTest.ogg");
	AudioMixer::GetSingleton().PlayMusic();
}

void ExampleScene::OnExit() {
	//AudioMixer::GetSingleton().UnloadChunk("music");
	AudioMixer::GetSingleton().UnloadMusic();
}

//frame phases
void ExampleScene::OnFrameStart() {
	//
}

void ExampleScene::OnUpdate() {
	Vector2 gravity = {0, 0.1};
	double friction = 0.01;

	auto actors = root.GetChildrenByType<NodeActor>();

	for (auto it = actors.begin(); it != actors.end(); it++) {
		(*it)->Update(gravity, friction);
	}
}

void ExampleScene::OnFrameEnd() {
	root.GetChild(1)->GetFirstChildByType<NodeColliderBox>()->SnapCollide( *(root.GetChild(0)->GetFirstChildByType<NodeColliderBox>()) );
}

void ExampleScene::OnRenderFrame(SDL_Renderer* renderer) {
	auto images = root.GetChildrenByType<NodeImage>();

	for (auto it = images.begin(); it != images.end(); it++) {
		(*it)->DrawTo(renderer, camera.GetPosition()->x, camera.GetPosition()->y, camera.GetScale()->x, camera.GetScale()->y);
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
	camera.MouseWheel(event, GetRenderer());

	//guard
	if (!AudioMixer::GetSingleton().GetChunkLoaded("music")) {
		return;
	}

	AudioMixer::GetSingleton().SetChannelVolume(musicChannel, std::min(camera.GetScale()->x * 16, 128.0));
}

void ExampleScene::OnKeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			SetSceneSignal(SceneSignal::POP);
		break;

		case SDLK_SPACE:
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion()->y = -8;
		break;

		case SDLK_LEFT:
		case SDLK_a:
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion()->x = -1;
		break;

		case SDLK_RIGHT:
		case SDLK_d:
			root.GetChild(1)->GetFirstChildByType<NodeTransform>()->GetMotion()->x = 1;
		break;

		case SDLK_m: {
			//guard
			if (!AudioMixer::GetSingleton().GetChunkLoaded("music")) {
				break;
			}

			if (AudioMixer::GetSingleton().GetChannelPlaying(musicChannel)) {
				//toggle
				if (AudioMixer::GetSingleton().GetChannelPaused(musicChannel)) {
					AudioMixer::GetSingleton().UnpauseChannel(musicChannel);
				} else {
					AudioMixer::GetSingleton().PauseChannel(musicChannel);
				}
			} else {
				musicChannel = AudioMixer::GetSingleton().PlayChunk("music");
			}
		}
		break;
	}
}

void ExampleScene::OnKeyUp(SDL_KeyboardEvent const& event) {
	//
}

void ExampleScene::OnControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void ExampleScene::OnControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void ExampleScene::OnControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}