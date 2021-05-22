#pragma once

#include "image.hpp"
#include "node_base.hpp"

class NodeImage : public NodeBase, public Image {
public:
	NodeImage(SDL_Renderer* renderer, std::string str) : NodeBase(), Image(renderer, str) {}

	void DrawTo(SDL_Renderer* const, Sint16 x, Sint16 y, double scaleX = 1.0, double scaleY = 1.0);

protected:
	using Image::DrawTo;
};