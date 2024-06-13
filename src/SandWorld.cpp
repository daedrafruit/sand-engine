#include <vector>
#include <SDL.h>

#include "RenderWindow.hpp"
#include "SandWorld.hpp"
#include "Entity.hpp"


SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int p_cellSize)
	:gridHeight(p_windowHeight / p_cellSize), gridWidth(p_windowWidth / p_cellSize), cellSize(p_cellSize),
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

void SandWorld::updateWorld() {

	currWorldUpdate = (!currWorldUpdate);

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = 0; y < gridHeight; ++y) {

			bool hasBeenUpdated = grid[x][y].getLastUpdated() == currWorldUpdate;

			if (y == gridHeight - 1 && !(grid[x][y].getId() == 1)) {
				grid[x][y].setId(0);
			}

			if (hasBeenUpdated) {
				continue;	
			}
			
			if (grid[x][y].isEmpty() || grid[x][y].getId() == 1) {
				continue;
			}


			if (grid[x][y + 1].isEmpty()) {
				grid[x][y].setId(0);
				grid[x][y + 1].setId(2);
				grid[x][y + 1].setLastUpdated(currWorldUpdate);
			}

			else if (x > 0 && grid[x - 1][y + 1].isEmpty()) {
				grid[x][y].setId(0);
				grid[x - 1][y + 1].setId(2);
				grid[x - 1][y + 1].setLastUpdated(currWorldUpdate);
			} 

			else if (x < gridWidth - 1 && grid[x + 1][y + 1].isEmpty()) {
				grid[x][y].setId(0);
				grid[x + 1][y + 1].setId(2);
				grid[x + 1][y + 1].setLastUpdated(currWorldUpdate);
			}
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
                    cell.setId(0);
                }
            }
        }
    }
}

