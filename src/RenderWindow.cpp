
#include <iostream>
#include <SDl.h>
#include <numeric>

#include "SDL_pixels.h"
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

	const int partitionSideLength = p_world.getPartitionSideLength();
	const int cellSize = p_world.getCellSize();
	const int partitionWidth = p_world.getPartitionWidth();
	const int partitionHeight = p_world.getPartitionHeight();
	
	/*
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for (int col = 0; col <= partitionSideLength; ++col) {
		int x = col * partitionWidth * cellSize;
		SDL_RenderDrawLine(renderer, x, 0, x, cellSize * partitionSideLength * partitionHeight);
	}

	for (int row = 0; row <= partitionSideLength; ++row) {
		int y = row * partitionHeight * cellSize;
		SDL_RenderDrawLine(renderer, 0, y, cellSize * partitionSideLength * partitionWidth, y);
	}
*/

	for (int x = 0; x < partitionSideLength; ++x) {
		for (int y = 0; y < partitionSideLength; ++y) {
			if (p_world.partitionActive(x, y)) {
				/*
				SDL_Rect rect = { x  * partitionWidth, y *  partitionHeight, cellSize, cellSize};

				SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderFillRect(renderer, &rect);
*/
				renderPartition(x, y, p_world);

			} else {
				/*
				SDL_Rect rect = { x  *  partitionWidth, y *  partitionHeight, cellSize, cellSize};

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderFillRect(renderer, &rect);
*/
				//renderPartition(x, y, p_world);
			}
		}
	}
}

void RenderWindow::renderPartition(int p_x, int p_y, const SandWorld& p_world) {

	const int partitionWidth = p_world.getPartitionWidth();
	const int partitionHeight = p_world.getPartitionHeight();

	int xi = p_x * partitionWidth;
	int yi = p_y * partitionHeight;

	int xf = (xi + partitionWidth);
	int yf = (yi + partitionHeight);

	const int cellSize = p_world.getCellSize();

	for (int x = xi; x < xf; ++x) {
		for (int y = yi; y < yf; ++y) {
			const int gridX = x * cellSize;
			const int gridY = y * cellSize;
			const Entity& cell = p_world.getCellAt(x, y);

			render(cell, gridX, gridY, cellSize);
		}
	}
}



