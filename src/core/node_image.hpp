#pragma once

#include "image.hpp"
#include "node_base.hpp"

class NodeImage : public NodeBase, public Image {
public:
	NodeImage(SDL_Renderer* renderer, std::string str) : NodeBase(), Image(renderer, str) {}
};