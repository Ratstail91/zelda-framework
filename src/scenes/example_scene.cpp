#include "example_scene.hpp"

ExampleScene::ExampleScene() {
	//fill the tree
	root.AddChild(new NodeBase());
	root.AddChild(new NodeBase());

	//ground
	root.GetChild(0)->AddChild(new NodeTransform());

	((NodeTransform*)(root.GetChild(0)->GetChild(-1)))->GetPosition()->y = 400;

	root.GetChild(0)->AddChild(new NodeImage(GetRenderer(), "rsc/thing.png"));
	root.GetChild(0)->AddChild(new NodeColliderBox());

	((NodeColliderBox*)(root.GetChild(0)->GetChild(-1)))->SetBoundsToImageSibling();

	//dragon
	root.GetChild(1)->AddChild(new NodeTransform());
	root.GetChild(1)->AddChild(new NodeImage(GetRenderer(), "rsc/fairy dragon.png"));
	root.GetChild(1)->AddChild(new NodeColliderBox());

	((NodeColliderBox*)(root.GetChild(1)->GetChild(-1)))->SetBoundsToImageSibling();

	root.GetChild(1)->AddChild(new NodeActor());
}

ExampleScene::~ExampleScene() {
	//EMPTY
}

//frame phases
void ExampleScene::FrameStart() {
	//
}

void ExampleScene::Update() {
	Vector2 gravity = {0, 0.1};
	double friction = 0.01;

	auto actors = root.GetChildrenByType<NodeActor>();

	for (auto it = actors.begin(); it != actors.end(); it++) {
		(*it)->Update(gravity, friction);
	}
}

void ExampleScene::FrameEnd() {
	root.GetChild(1)->GetFirstChildByType<NodeColliderBox>()->SnapCollide( *(root.GetChild(0)->GetFirstChildByType<NodeColliderBox>()) );
}

void ExampleScene::RenderFrame(SDL_Renderer* renderer) {
	Vector2 camera = {0, 0};

	auto images = root.GetChildrenByType<NodeImage>();

	for (auto it = images.begin(); it != images.end(); it++) {
		(*it)->DrawTo(renderer, camera.x, camera.y);
	}
}

//input events
void ExampleScene::MouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void ExampleScene::MouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void ExampleScene::MouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void ExampleScene::KeyDown(SDL_KeyboardEvent const& event) {
	//preference as a default
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			QuitEvent();
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
	}
}

void ExampleScene::KeyUp(SDL_KeyboardEvent const& event) {
	//
}

void ExampleScene::ControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void ExampleScene::ControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void ExampleScene::ControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}