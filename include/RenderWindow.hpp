
#pragma once
#include <SDL2/SDL.h>
#include <memory>

#include "Entity.hpp"
#include "SDL_render.h"
#include "SDL2/SDL_ttf.h"
#include "SandWorld.hpp"

class RenderWindow {

public:
	RenderWindow(const char* title, const int p_width, const int p_height, const SandWorld& p_world);

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height;	}

	void cleanUp();
	void clear();
	void render(const std::unique_ptr<Entity>& entity, int x, int y, int cellSize);
	void display();
	void handleEvent(const Uint8* currKeyStates, int p_x, int p_y);

	void renderWorld(const SandWorld& world);
	void renderDebug(const int fps);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	TTF_Font* font;
	bool showDebug;

	const int width, height;
	const SandWorld& world;
};
