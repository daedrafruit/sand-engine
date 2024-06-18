#include <algorithm>
#include <ios>
#include <random>
#include <vector>
#include <SDL.h>

#include "RenderWindow.hpp"
#include "SandWorld.hpp"
#include "Entity.hpp"
#include "SDL_timer.h"
#include "utils.hpp"


SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int gridSize)
	:gridHeight(p_windowHeight / gridSize), gridWidth(p_windowWidth / gridSize), cellSize(gridSize),
	 grid(gridWidth, std::vector<Entity>(gridHeight, Entity(0))),
	 currWorldUpdate(SDL_GetTicks()) {

	drawBarrier();
}

void SandWorld::drawBarrier() {
	//top & bottom
	for (int x = 0; x < gridWidth; x++) {
		grid[x][0].setId(1, currWorldUpdate);
		grid[x][gridHeight - 1].setId(1, currWorldUpdate);
	}

	//right & left
	for (int y = 0; y < gridHeight; y++) {
		grid[0][y].setId(1, currWorldUpdate);
		grid[gridWidth - 1][y].setId(1, currWorldUpdate);
	}
}

void SandWorld::renderWorld(RenderWindow& p_window) {
	for (int x = 0; x < gridWidth; x++) {
		for (int y = 0; y < gridHeight; y++) {
			const int gridX = x * cellSize;
			const int gridY = y * cellSize;

			p_window.render(grid[x][y], gridX, gridY, cellSize);
		}
	}
}

void SandWorld::mouseEvent(const RenderWindow& p_window) {
	int radius = 3;

	Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	bool withinBounds = mouseX >= (radius * cellSize) && mouseX < p_window.getWidth() - (radius * cellSize) &&
						mouseY >= (radius * cellSize) && mouseY < p_window.getHeight() - (radius * cellSize);

	if (!withinBounds) {
		return;
	}

	//draw circle in radius
	for (int dx = -radius; dx <= radius; ++dx) {
		for (int dy = -radius; dy <= radius; ++dy) {
			Entity& cell = grid[(mouseX / cellSize) + dx][(mouseY / cellSize) + dy];

			bool withinRadius = dx * dx + dy * dy <= radius * radius;
			bool leftClick = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
			bool rightClick = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT));

			if (withinRadius) {
				if (leftClick) {
					cell.setId(2, currWorldUpdate);
				} else if (rightClick) {
					cell.setId(3, currWorldUpdate);
				}
			}
		}
	}
}

void SandWorld::updateWorld() {

	currWorldUpdate = (SDL_GetTicks());

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = gridHeight - 1; y >= 0; --y) {

			if (y == gridHeight - 1 && grid[x][y].getId() > 1) {
				grid[x][y].setId(0, currWorldUpdate);
				continue;
			}

			switch (grid[x][y].getId()) {
				case 0:
					continue;
					break;
				case 1:
					continue;
					break;
				case 2:
					updateSand(x, y);
					break;
				case 3:
					updateWater(x, y);
					break;
			}
		}
	}
	commitSwaps();
}

void SandWorld::commitSwaps() {
	std::shuffle(swaps.begin(), swaps.end(), utils::getRandomEngine());
	for (const SwapOperation& swap : swaps) {
		Entity& cell1 = grid[swap.x1][swap.y1];
		Entity& cell2 = grid[swap.x2][swap.y2];

		std::swap(cell1, cell2);
	}

	swaps.clear();
}

void SandWorld::updateSand(int x, int y) {

	const SwapOperation below = {x, y, x, y+1};
	const SwapOperation downLeft = {x, y, x-1, y+1};
	const SwapOperation downRight = {x, y, x+1, y+1};

	SwapOperation checkCells[3] = {below, downLeft, downRight};
	std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

	for (const SwapOperation& swap : checkCells) {
		Entity& cell1 = grid[swap.x1][swap.y1];
		Entity& cell2 = grid[swap.x2][swap.y2];

		bool cellsHaveBeenUpdated = (cell2.getLastUpdated() == currWorldUpdate && cell1.getLastUpdated() == currWorldUpdate);

		if (cell2.isEmpty() || cell2.getId() == 3) {
			cell1.setLastUpdated(currWorldUpdate);
			cell2.setLastUpdated(currWorldUpdate);
			swaps.push_back(swap); 
			break;
		}
	}
}

void SandWorld::updateWater(int x, int y) {

	const SwapOperation below = {x, y, x, y+1};
	const SwapOperation downLeft = {x, y, x-1, y+1};
	const SwapOperation downRight = {x, y, x+1, y+1};
	const SwapOperation left = {x, y, x-1, y};
	const SwapOperation right = {x, y, x+1, y};

	SwapOperation checkCells[5] = {below, downLeft, downRight, left, right};
	std::shuffle(checkCells + 1, checkCells + 5, utils::getRandomEngine());
	for (const SwapOperation& swap : checkCells) {
		Entity& cell1 = grid[swap.x1][swap.y1];
		Entity& cell2 = grid[swap.x2][swap.y2];

		bool cellsHaveBeenUpdated = (cell2.getLastUpdated() == currWorldUpdate && cell1.getLastUpdated() == currWorldUpdate);
		if (cellsHaveBeenUpdated) continue;

		if (cell2.isEmpty()) {
			cell1.setLastUpdated(currWorldUpdate);
			cell2.setLastUpdated(currWorldUpdate);
			swaps.push_back(swap); 
			break;
		}
	}
}


