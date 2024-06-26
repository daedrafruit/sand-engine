#include <algorithm>
#include <vector>
#include <SDL.h>

#include "SandWorld.hpp"
#include "Entity.hpp"
#include "SDL_timer.h"
#include "utils.hpp"


SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int gridSize)
	:gridHeight(p_windowHeight / gridSize), gridWidth(p_windowWidth / gridSize), cellSize(gridSize),
	 grid(gridWidth, std::vector<Entity>(gridHeight, Entity(CellId::Air))),
	 currWorldUpdate(SDL_GetTicks()) {

	drawBarrier();
}

void SandWorld::drawBarrier() {
	//top & bottom
	for (int x = 0; x < gridWidth; x++) {
		grid[x][0].setId(CellId::Stone, currWorldUpdate);
		grid[x][gridHeight - 1].setId(CellId::Stone, currWorldUpdate);
	}

	//right & left
	for (int y = 0; y < gridHeight; y++) {
		grid[0][y].setId(CellId::Stone, currWorldUpdate);
		grid[gridWidth - 1][y].setId(CellId::Stone, currWorldUpdate);
	}
}

void SandWorld::handleEvent(Event p_event, int p_x, int p_y) {
	const int x = p_x / cellSize;
	const int y = p_y / cellSize;

	switch (p_event) {
		case Event::leftMouse:
			drawCircle(x, y, 3, CellId::Sand);
			break;
		case Event::rightMouse:
			drawCircle(x, y, 3, CellId::Stone);
			break;
	}	

}

void SandWorld::drawCircle(int p_x, int p_y, int radius, CellId p_id) {

	bool withinBounds = p_x >= radius && p_x < gridWidth - radius && p_y >= radius && p_y < gridHeight - radius;

	if (!withinBounds) {
		return;
	}

	grid[p_x][p_y].setId(p_id, currWorldUpdate);

	for (int dx = -radius; dx <= radius; ++dx) {
		for (int dy = -radius; dy <= radius; ++dy) {
			Entity& cell = grid[p_x + dx][p_y + dy];

			// Quadratic formula
			bool withinRadius = dx * dx + dy * dy <= radius * radius;

			if (withinRadius) cell.setId(p_id, currWorldUpdate);
		}
	}
}


void SandWorld::updateWorld() {

	currWorldUpdate = (SDL_GetTicks());

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = gridHeight - 1; y >= 0; --y) {
			bool cellIsAtPerimeter = (x == 0 || x == gridWidth || y == 0 || y == gridHeight - 1) && grid[x][y].getId() > CellId::Stone;

			if (cellIsAtPerimeter) {
				grid[x][y].setId(CellId::Air, currWorldUpdate);
				continue;
			}

			switch (grid[x][y].getId()) {
				case CellId::Air:
					continue;
					break;
				case CellId::Stone:
					continue;
			break;
				case CellId::Sand:
					updateSand(x, y);
					break;
				case CellId::Water:
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
		bool cellPrevUpdated = cell1.getLastUpdated() == currWorldUpdate || cell2.getLastUpdated() == currWorldUpdate;

		if (cellPrevUpdated) {
			continue;
		}

		cell1.setLastUpdated(currWorldUpdate);
		cell2.setLastUpdated(currWorldUpdate);

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

		if (cell2.isEmpty() || cell2.getId() == CellId::Water) {
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

		if (cell2.isEmpty()) {
			swaps.push_back(swap); 
			break;
		}
	}
}


