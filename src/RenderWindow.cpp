
#include <cstddef>
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>

#include "SDL2/SDL_pixels.h"
#include "SDL_error.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SandWorld.hpp"
#include "Entity.hpp"
#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* title, int p_width, int p_height, const SandWorld& p_world)
	:window(NULL), renderer(NULL), width(p_width), height(p_height), world(p_world) {

	window = SDL_CreateWindow(title, 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			p_width, p_height, SDL_WINDOW_SHOWN
	);
	if (!window) {
		std::cout << "Window failed to init" << SDL_GetError() << std::endl; 
  }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cout << "Renderer failed to init" << SDL_GetError() << std::endl; 
  }

	texture = SDL_CreateTexture(renderer, 
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 
			width, height
	);
	if (!texture) {
		std::cout << "Texture failed to init" << SDL_GetError() << std::endl; 
  }

} 

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(const std::unique_ptr<Entity>& entity, int x, int y, int cellSize) {

	SDL_Rect rect = { x, y, cellSize, cellSize};

	Color color = entity->getColor();

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

void RenderWindow::renderWorld(const SandWorld& world) {
	Uint32* pixels;
	int pitch;

	if (SDL_LockTexture(texture, nullptr, (void **)&pixels, &pitch)) {
		std::cout << "Failed to lock texture: " << SDL_GetError() << std::endl;
	}

	SDL_PixelFormat pixelFormat;
	pixelFormat.format = SDL_PIXELFORMAT_RGBA8888;

	for (int x = 0; x < world.getGridWidth(); ++x) {
		for (int y = 0; y < world.getGridHeight(); ++y) {
			const std::unique_ptr<Entity>& cell = world.getCellAt(x, y);
			Color cellColor = cell->getColor();
			Uint32 color = SDL_MapRGBA(&pixelFormat, 
					cellColor.r, cellColor.g, cellColor.b, SDL_ALPHA_OPAQUE
			);
			pixels[y * (pitch/sizeof(unsigned int)) + x] = color;

		}
	}

	SDL_UnlockTexture(texture);
	SDL_Rect rect = {0, 0, 
		width * world.getCellSize(),
		height * world.getCellSize()
	};

	SDL_RenderCopy(renderer, texture, NULL, &rect);
}
