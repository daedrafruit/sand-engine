
#pragma once
#include <SDL3/SDL.h>
#include <memory>

#include "Entity.hpp"
#include "SDL_render.h"
//#include "SDL3/SDL_ttf.h"
#include "SandWorld.hpp"

class RenderWindow {

public:
	RenderWindow(const char* title, const int p_width, const int p_height);

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height;	}

	void cleanUp();
	void clear();
	void display();
	void handleEvent(const bool* currKeyStates, int p_x, int p_y);

	void renderWorld(const SandWorld& world);
	void renderPartitionBorders(const SandWorld& world, int x, int y, Uint32* pixels, int pitch);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
//	TTF_Font* font;
	bool showDebug;

	const int width, height;
};
