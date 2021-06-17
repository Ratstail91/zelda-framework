#include "pong_scene.hpp"

#include <iostream>

PongScene::PongScene() {
	//generate the player objects
	playerOne = GeneratePaddle({-200, 0});
	playerTwo = GeneratePaddle({200, 0});

	root.AddChild(playerOne);
	root.AddChild(playerTwo);

	root.AddChild(GenerateWall({0, -150}));
	root.AddChild(GenerateWall({0, 150}));

	ball = GenerateBall({0, 0});

	root.AddChild(ball);

	//inital movement
	*( ball->GetFirstChildByType<NodeTransform>()->GetMotion() ) = {2, 2};
}

PongScene::~PongScene() {
	removeDescendantsOfNode(&root);
}

//frame phases
void PongScene::OnFrameStart() {
	//
}

void PongScene::OnUpdate() {
	//update the actors
	Vector2 gravity = {0, 0};
	double friction = 0.0;

	auto actors = root.GetDescendantsByType<NodeActor>();
	for (auto actorPtr : actors) {
		actorPtr->Update(gravity, friction);
	}
}

void PongScene::OnFrameEnd() {
	auto boxes = root.GetDescendantsByType<NodeColliderBox>();

	//bounce the ball against all objects
	NodeColliderBox* ballBox = ball->GetFirstChildByType<NodeColliderBox>();
	for (auto box : boxes) {
		if (ballBox->Intersect(*box)) {
			//who did I collide with?
			if (playerOne->GetFirstChildByType<NodeColliderBox>()->Intersect(*ballBox) || playerTwo->GetFirstChildByType<NodeColliderBox>()->Intersect(*ballBox)) {
				//you collided with a paddle; bounce back faster
				ball->GetFirstChildByType<NodeTransform>()->GetMotion()->x *= -1.1;

				//simulate one step out of the paddle
				ball->GetFirstChildByType<NodeActor>()->Update({0, 0}, 0.0);

				//BUGFIX: keep the ball out of the paddle sides
				if (playerOne->GetFirstChildByType<NodeColliderBox>()->Intersect(*ballBox) || playerTwo->GetFirstChildByType<NodeColliderBox>()->Intersect(*ballBox)) {
					//undo the update
					ball->GetFirstChildByType<NodeActor>()->Rewind({0, 0}, 0.0);

					//reverse both directions (flipping the prior reverse)
					*( ball->GetFirstChildByType<NodeTransform>()->GetMotion() ) *= -1;

					//redo the update
					ball->GetFirstChildByType<NodeActor>()->Update({0, 0}, 0.0);
				}
			} else {
				//you collided with a regular wall
				ball->GetFirstChildByType<NodeTransform>()->GetMotion()->y *= -1;
			}
		}
	}

	//correct for the paddle collisions
	for (auto box : boxes) {
		if (box == ballBox) {
			continue;
		}

		playerOne->GetFirstChildByType<NodeColliderBox>()->SnapCollide( *box );
		playerTwo->GetFirstChildByType<NodeColliderBox>()->SnapCollide( *box );
	}
}

void PongScene::OnRenderFrame(SDL_Renderer* renderer) {
	//get the screen size
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);

	//iterate over all images
	auto images = root.GetDescendantsByType<NodeImage>();

	for (auto imageNode : images) {
		imageNode->DrawTo(renderer, w/2, h/2, 1, 1);
	}
}

//input events
void PongScene::OnMouseMotion(SDL_MouseMotionEvent const& event) {
	//
}

void PongScene::OnMouseButtonDown(SDL_MouseButtonEvent const& event) {
	//
}

void PongScene::OnMouseButtonUp(SDL_MouseButtonEvent const& event) {
	//
}

void PongScene::OnMouseWheel(SDL_MouseWheelEvent const& event) {
	//
}

void PongScene::OnKeyDown(SDL_KeyboardEvent const& event) {
	switch(event.keysym.sym) {
		case SDLK_ESCAPE:
			SetSceneSignal(SceneSignal::POP);
		break;

		case SDLK_w:
			playerOne->GetFirstChildByType<NodeTransform>()->GetMotion()->y -= 3;
		break;

		case SDLK_s:
			playerOne->GetFirstChildByType<NodeTransform>()->GetMotion()->y += 3;
		break;

		case SDLK_UP:
			playerTwo->GetFirstChildByType<NodeTransform>()->GetMotion()->y -= 3;
		break;

		case SDLK_DOWN:
			playerTwo->GetFirstChildByType<NodeTransform>()->GetMotion()->y += 3;
		break;
	}
}

void PongScene::OnKeyUp(SDL_KeyboardEvent const& event) {
	switch(event.keysym.sym) {
		case SDLK_w: {
			NodeTransform* transform = playerOne->GetFirstChildByType<NodeTransform>();
			if (transform->GetMotion()->y < 0) {
				transform->GetMotion()->y = 0;
			}
		}
		break;

		case SDLK_s: {
			NodeTransform* transform = playerOne->GetFirstChildByType<NodeTransform>();
			if (transform->GetMotion()->y > 0) {
				transform->GetMotion()->y = 0;
			}
		}
		break;

		case SDLK_UP: {
			NodeTransform* transform = playerTwo->GetFirstChildByType<NodeTransform>();
			if (transform->GetMotion()->y < 0) {
				transform->GetMotion()->y = 0;
			}
		}
		break;

		case SDLK_DOWN: {
			NodeTransform* transform = playerTwo->GetFirstChildByType<NodeTransform>();
			if (transform->GetMotion()->y > 0) {
				transform->GetMotion()->y = 0;
			}
		}
		break;
	}
}

void PongScene::OnControllerAxisMotion(SDL_ControllerAxisEvent const& event) {
	//
}

void PongScene::OnControllerButtonDown(SDL_ControllerButtonEvent const& event) {
	//
}

void PongScene::OnControllerButtonUp(SDL_ControllerButtonEvent const& event) {
	//
}

//bespoke methods
NodeBase* PongScene::GeneratePaddle(Vector2 position) {
	NodeBase* paddleNode = new NodeBase();

	paddleNode->AddChild(new NodeTransform());
	paddleNode->AddChild(new NodeImage(GetRenderer(), "rsc/pong_paddle.png"));
	paddleNode->AddChild(new NodeColliderBox());
	paddleNode->AddChild(new NodeActor());

	//correct the collision box
	paddleNode->GetFirstChildByType<NodeColliderBox>()->SetBoundsToImageSibling(); //TODO: could this be automatic?

	//correct the position
	*( paddleNode->GetFirstChildByType<NodeTransform>()->GetPosition() ) = position - paddleNode->GetFirstChildByType<NodeColliderBox>()->center;

	return paddleNode;
}

NodeBase* PongScene::GenerateWall(Vector2 position) {
	NodeBase* wallNode = new NodeBase();

	wallNode->AddChild(new NodeTransform());
	wallNode->AddChild(new NodeImage(GetRenderer(), "rsc/pong_wall.png"));
	wallNode->AddChild(new NodeColliderBox());

	//correct the collision box
	wallNode->GetFirstChildByType<NodeColliderBox>()->SetBoundsToImageSibling(); //TODO: could this be automatic?

	//correct the position
	*( wallNode->GetFirstChildByType<NodeTransform>()->GetPosition() ) = position - wallNode->GetFirstChildByType<NodeColliderBox>()->center;

	return wallNode;
}

NodeBase* PongScene::GenerateBall(Vector2 position) {
	NodeBase* ballNode = new NodeBase();

	ballNode->AddChild(new NodeTransform());
	ballNode->AddChild(new NodeImage(GetRenderer(), "rsc/pong_ball.png"));
	ballNode->AddChild(new NodeColliderBox());
	ballNode->AddChild(new NodeActor());

	//correct the collision box
	ballNode->GetFirstChildByType<NodeColliderBox>()->SetBoundsToImageSibling(); //TODO: could this be automatic?

	//correct the position
	*( ballNode->GetFirstChildByType<NodeTransform>()->GetPosition() ) = position - ballNode->GetFirstChildByType<NodeColliderBox>()->center;

	return ballNode;
}