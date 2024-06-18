
#include <iostream>
#include <SDl.h>

#include "SandWorld.hpp"
#include "Entity.hpp"
#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL), width(p_w), height(p_h) {

	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		std::cout << "Window failed to init" << SDL_GetError() << std::endl; 
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
} 

const int RenderWindow::getWidth() const {
	return width;
}

const int RenderWindow::getHeight() const {
	return height;
}

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(const Entity& p_entity, const int p_x, const int p_y, const int& p_cellSize) {

	SDL_Rect rect = { p_x, p_y, p_cellSize, p_cellSize};

	Color color = p_entity.getColor();

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

void RenderWindow::renderWorld(const SandWorld& p_world) {
	const int gridWidth = p_world.getGridWidth();
	const int gridHeight = p_world.getGridHeight();
	const int cellSize = p_world.getCellSize();
	const std::vector<std::vector<Entity>>& grid = p_world.getGrid(); 
	
	for (int x = 0; x < gridWidth; x++) {
		for (int y = 0; y < gridHeight; y++) {
			const int gridX = x * cellSize;
			const int gridY = y * cellSize;
			const Entity& cell = grid[x][y];

			render(cell, gridX, gridY, cellSize);
		}
	}
}
