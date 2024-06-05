#include <vector>
#include <SDL.h>

#include "RenderWindow.hpp"
#include "SandWorld.hpp"
#include "Entity.hpp"


SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int p_cellSize)
	:gridHeight(p_windowHeight / p_cellSize), gridWidth(p_windowWidth / p_cellSize), cellSize(p_cellSize),
	 grid(gridWidth, std::vector<Entity>(gridHeight, Entity(0))),
	 bufferGrid(gridWidth, std::vector<Entity>(gridHeight, Entity(0))) {

}

void SandWorld::renderWorld(RenderWindow p_window) {
	for (int x = 0; x < gridWidth; x++) {
		for (int y = 0; y < gridHeight; y++) {
			const int gridX = x * cellSize;
			const int gridY = y * cellSize;

			p_window.render(grid[x][y], gridX, gridY, cellSize);
		}
	}
}

void SandWorld::updateWorld() {
	copyGrid(grid, bufferGrid);

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = 0; y < gridHeight - 1; ++y) {
			if (!grid[x][y].isEmpty()) {
				if (grid[x][y+1].isEmpty()) {
					bufferGrid[x][y].setId(0);
					bufferGrid[x][y+1].setId(1);
				} else if (x > 0 && grid[x-1][y+1].isEmpty()) {
					bufferGrid[x][y].setId(0);
					bufferGrid[x-1][y+1].setId(1);
				} else if (x < gridWidth - 1 && grid[x+1][y+1].isEmpty()) {
					bufferGrid[x][y].setId(0);
					bufferGrid[x+1][y+1].setId(1);
				}
			}
		}
	}
	copyGrid(bufferGrid, grid);
}

void SandWorld::mouseEvent(RenderWindow p_window) {
	int mouseX, mouseY;
	Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX < p_window.getWidth() - 5 && mouseX > 5 && mouseY < p_window.getHeight() - 5 && mouseY > 5) {
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			grid[mouseX / cellSize][mouseY / cellSize].setId(1);
			grid[mouseX / cellSize + 1][mouseY / cellSize].setId(1);
			grid[mouseX / cellSize - 1][mouseY / cellSize].setId(1);
			grid[mouseX / cellSize][mouseY / cellSize + 1].setId(1);
			grid[mouseX / cellSize][mouseY / cellSize - 1].setId(1);
		}

		if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			grid[mouseX / cellSize][mouseY / cellSize].setId(0);
			grid[mouseX / cellSize + 1][mouseY / cellSize].setId(0);
			grid[mouseX / cellSize - 1][mouseY / cellSize].setId(0);
			grid[mouseX / cellSize][mouseY / cellSize + 1].setId(0);
			grid[mouseX / cellSize][mouseY / cellSize - 1].setId(0);
		}
	}
}

void SandWorld::copyGrid(const std::vector<std::vector<Entity>>& source, std::vector<std::vector<Entity>>& destination) {
	for (size_t i = 0; i < source.size(); ++i) {
		for (size_t j = 0; j < source[i].size(); ++j) {
			destination[i][j] = source[i][j];
		}
	}
}
