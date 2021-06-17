#include "image.hpp"

#include "SDL2/SDL_image.h"

#include <sstream>
#include <stdexcept>

using namespace std::string_literals;

static void error(const std::string& str) {
	std::ostringstream msg;
	msg << str << ": " << SDL_GetError();
	throw(std::runtime_error(msg.str()));
}

Image& Image::operator=(Image const& rhs) {
	//don't screw yourself
	if (this == &rhs) {
		return *this;
	}

	Free();

	//Copy the other Image's stuff
	texture = rhs.texture;
	clip = rhs.clip;
	local = false;

	return *this;
}

Image& Image::operator=(Image&& rhs) {
	//don't screw yourself
	if (this == &rhs) {
		return *this;
	}

	Free();

	//Steal the other Image's stuff
	texture = rhs.texture;
	clip = rhs.clip;
	local = rhs.local;

	rhs.texture = nullptr;
	rhs.clip = {0, 0, 0, 0};
	rhs.local = false;

	return *this;
}

SDL_Texture* Image::Load(SDL_Renderer* const renderer, std::string const& fname) {
	Free();

	//load the file into a surface
	SDL_Surface* surface = IMG_Load(fname.c_str());
	if (!surface) {
		error("Failed to load an image file "s + fname);
	}

	//create a texture from this surface
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		error("Failed to convert a newly loaded image from a surface to a texture "s + fname);
	}

	//set the metadata
	clip.x = 0;
	clip.y = 0;
	if (SDL_QueryTexture(texture, nullptr, nullptr, &clip.w, &clip.h)) {
		error("Failed to record metadata for a newly loaded image "s + fname);
	}
	local = true;

	//free the surface & return
	SDL_FreeSurface(surface);
	return texture;
}

SDL_Texture* Image::Create(SDL_Renderer* const renderer, Uint16 w, Uint16 h, SDL_Color blank) {
	Free();

	//make the texture
	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		w, h);

	//check
	if (!texture) {
		error("Failed to create a texture for an image"s);
	}

	//set the metadata
	clip.x = 0;
	clip.y = 0;
	if (SDL_QueryTexture(texture, nullptr, nullptr, &clip.w, &clip.h)) {
		error("Failed to record metadata for a newly created image"s);
	}
	local = true;

	//blank (black) texture
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, blank.r, blank.g, blank.b, blank.a);
	SDL_RenderFillRect(renderer, nullptr);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRenderTarget(renderer, nullptr);

	return texture;
}

SDL_Texture* Image::CopyTexture(SDL_Renderer* const renderer, SDL_Texture* const ptr) {
	Free();
	int w = 0, h = 0;

	//get the info
	SDL_QueryTexture(ptr, nullptr, nullptr, &w, &h);

	//create a texture of (w, h) size (also sets the metadata)
	Create(renderer, w, h);

	//copy the argument texture to the local texture
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderCopy(renderer, ptr, nullptr, nullptr);
	SDL_SetRenderTarget(renderer, nullptr);

	//return the local texture
	return texture;
}

SDL_Texture* Image::SetTexture(SDL_Texture* ptr) {
	Free();

	texture = ptr;

	//set the metadata
	clip.x = 0;
	clip.y = 0;
	if (SDL_QueryTexture(texture, nullptr, nullptr, &clip.w, &clip.h)) {
		error("Failed to record metadata for a newly set image"s);
	}
	local = false;

	return texture;
}

SDL_Texture* Image::GetTexture() const {
	return texture;
}

void Image::Free() {
	if (local) {
		SDL_DestroyTexture(texture);
		local = false;
	}
	texture = nullptr;
	clip = {0, 0, 0, 0};
}

void Image::DrawTo(SDL_Renderer* const renderer, Sint16 x, Sint16 y, double scaleX, double scaleY) {
	if (!texture) {
		error("No image texture to draw"s);
	}

	SDL_Rect sclip = clip;
	SDL_Rect dclip = {(Sint16)(x * scaleX), (Sint16)(y * scaleY), Uint16(clip.w * scaleX), Uint16(clip.h * scaleY)};
	SDL_RenderCopy(renderer, texture, &sclip, &dclip);
}

void Image::SetAlpha(Uint8 a) {
	if (SDL_SetTextureAlphaMod(texture, a)) {
		error("Failed to set alpha of an image"s);
	}
}

Uint8 Image::GetAlpha() {
	Uint8 ret = 0;
	if (SDL_GetTextureAlphaMod(texture, &ret)) {
		error("Failed to get alpha of an image"s);
	}
	return ret;
}