#include <algorithm>
#include <random>
#include <vector>
#include <SDL.h>

#include "RenderWindow.hpp"
#include "SandWorld.hpp"
#include "Entity.hpp"
#include "SDL_timer.h"


SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int gridSize)
	:gridHeight(p_windowHeight / gridSize), gridWidth(p_windowWidth / gridSize), cellSize(gridSize),
	 grid(gridWidth, std::vector<Entity>(gridHeight, Entity(0))),
	 currWorldUpdate(false) {

	drawBarrier();
}

void SandWorld::drawBarrier() {
	//top & bottom
	for (int x = 0; x < gridWidth; x++) {
		grid[x][0].setId(1);
		grid[x][gridHeight - 1].setId(1);
	}

	//right & left
	for (int y = 0; y < gridHeight; y++) {
		grid[0][y].setId(1);
		grid[gridWidth - 1][y].setId(1);
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
			bool cellIsEmpty = cell.isEmpty();

			if (withinRadius) {
				if (leftClick && cellIsEmpty) {
					cell.setId(2);
				} else if (rightClick) {
					cell.setId(3);
				}
			}
		}
	}
}

void SandWorld::updateWorld() {

	currWorldUpdate = (!currWorldUpdate);

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = gridHeight; y > 0; --y) {

			if (grid[x][y].getLastUpdated() == currWorldUpdate) {
				continue;	
			}

			grid[x][y].setLastUpdated(currWorldUpdate);

			if (y == gridHeight - 1 && grid[x][y].getId() > 1) {
				grid[x][y].setId(0);
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
}

void SandWorld::swapCells(Entity& cell1, Entity& cell2) {
	Entity temp = cell1;

	cell1 = cell2;
	cell2 = temp;

	//cell1.setLastUpdated(currWorldUpdate);
	//cell2.setLastUpdated(currWorldUpdate);
}

void SandWorld::updateSand(int x, int y) {
	Entity& curr = grid[x][y];

	Entity& below = grid[x][y+1];
	Entity& downLeft = grid[x-1][y+1];
	Entity& downRight = grid[x+1][y+1];

	int numChecks = 3;

	Entity* checkCells[3] = {&below, &downLeft, &downRight};

	std::shuffle(checkCells + 1, checkCells + numChecks, std::default_random_engine(SDL_GetTicks()));

	for (int i = 0; i < numChecks; i++) {
		if (checkCells[i]->isEmpty() || checkCells[i]->getId() == 3) {
			swapCells(curr, *checkCells[i]);
			break;
		}
	}
}

void SandWorld::updateWater(int p_x, int p_y) {
	Entity& curr = grid[p_x][p_y];

	Entity& below = grid[p_x][p_y+1];
	Entity& left = grid[p_x-1][p_y];
	Entity& right = grid[p_x+1][p_y];
	Entity& downLeft = grid[p_x-1][p_y+1];
	Entity& downRight = grid[p_x+1][p_y+1];

	int numChecks = 5;

	Entity* checkCells[5] = {&below, &downLeft, &downRight, &left, &right};

	std::shuffle(checkCells + 1, checkCells + numChecks, std::default_random_engine(SDL_GetTicks()));

	for (int i = 0; i < numChecks; i++) {
		if (checkCells[i]->isEmpty() || checkCells[i]->getId() == 2) {
			swapCells(curr, *checkCells[i]);
			break;
		}
	}
}

