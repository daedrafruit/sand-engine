
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>

#include "SDL2/SDL_pixels.h"
#include "SandWorld.hpp"
#include "Entity.hpp"
#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* title, int p_width, int p_height, int p_partitionSideLength)
	:window(NULL), renderer(NULL), width(p_width), height(p_height), partitionSideLength(p_partitionSideLength) {

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_width, p_height, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		std::cout << "Window failed to init" << SDL_GetError() << std::endl; 
  }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	renderPartitions.resize(partitionSideLength, std::vector<bool>(partitionSideLength, true));
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

	const int partitionSideLength = world.getPartitionSideLength();
	const int cellSize = world.getCellSize();
	const int partitionWidth = world.getPartitionWidth();
	const int partitionHeight = world.getPartitionHeight();
	
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
			SDL_Rect rect = { x  * partitionWidth , y * partitionHeight , cellSize, cellSize};
			if (renderPartitions[x][y]) {

				renderPartition(x, y, world);
				renderPartitions[x][y] = false;

				SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderFillRect(renderer, &rect);

			} else {

				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderFillRect(renderer, &rect);

				//renderPartition(x, y, p_world);
			}
		}
	}
}

void RenderWindow::renderPartition(int p_x, int p_y, const SandWorld& world) {

	const int partitionWidth = world.getPartitionWidth();
	const int partitionHeight = world.getPartitionHeight();

	int xi = p_x * partitionWidth;
	int yi = p_y * partitionHeight;

	int xf = (xi + partitionWidth);
	int yf = (yi + partitionHeight);

	const int cellSize = world.getCellSize();

	for (int x = xi; x < xf; ++x) {
		for (int y = yi; y < yf; ++y) {
			const int gridX = x * cellSize;
			const int gridY = y * cellSize;
			const std::unique_ptr<Entity>& cell = world.getCellAt(x, y);

			render(cell, gridX, gridY, cellSize);
		}
	}
}

void RenderWindow::updateRenderPartitions(std::vector<std::vector<bool>> worldPartitions) {
	for (int x = 0; x < partitionSideLength; ++x) {
		for (int y = 0; y < partitionSideLength; ++y) {
			renderPartitions[x][y] = worldPartitions[x][y] || renderPartitions[x][y];
		}
	}
}


