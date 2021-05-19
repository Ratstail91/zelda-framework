#pragma once

#include "SDL2/SDL.h"

#include <string>

class Image {
public:
	Image() = default;
	Image(Image const& rhs) { *this = rhs; }
	Image(Image&& rhs) { *this = std::move(rhs); }
	Image(SDL_Renderer* r, std::string fname) { Load(r, fname); }
	Image(SDL_Renderer* r, Uint16 w, Uint16 h) { Create(r, w, h); }
	Image(SDL_Texture* p) { SetTexture(p); }
	virtual ~Image() { Free(); }

	Image& operator=(Image const&);
	Image& operator=(Image&&);

	SDL_Texture* Load(SDL_Renderer* renderer, std::string fname);
	SDL_Texture* Create(SDL_Renderer* renderer, Uint16 w, Uint16 h, SDL_Color blank = {0, 0, 0, 255});
	SDL_Texture* CopyTexture(SDL_Renderer* renderer, SDL_Texture* ptr);
	SDL_Texture* SetTexture(SDL_Texture*);
	SDL_Texture* GetTexture() const;
	virtual void Free();

	void DrawTo(SDL_Renderer* const, Sint16 x, Sint16 y, double scaleX = 1.0, double scaleY = 1.0);

	void SetAlpha(Uint8 a);
	Uint8 GetAlpha();

	//Clip handlers
	SDL_Rect SetClip(SDL_Rect r) { return clip = r; }
	SDL_Rect GetClip() const { return clip; }

	Sint16 SetClipX(Sint16 x) { return clip.x = x; }
	Sint16 SetClipY(Sint16 y) { return clip.y = y; }
	Uint16 SetClipW(Uint16 w) { return clip.w = w; }
	Uint16 SetClipH(Uint16 h) { return clip.h = h; }

	Sint16 GetClipX() const { return clip.x; }
	Sint16 GetClipY() const { return clip.y; }
	Uint16 GetClipW() const { return clip.w; }
	Uint16 GetClipH() const { return clip.h; }

	bool GetLocal() const { return local; }

protected:
	SDL_Texture* texture = nullptr;
	SDL_Rect clip = {0, 0, 0, 0};
	bool local = false;
};