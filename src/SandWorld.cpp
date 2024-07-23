#include <algorithm>
#include <memory>
#include <vector>
#include <SDL.h>
#include <stdexcept>

#include "SandWorld.hpp"
#include "Entity.hpp"
#include "SDL_scancode.h"
#include "SDL_timer.h"
#include "utils.hpp"

// *********************************************************************
// Initialization
// *********************************************************************

SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int p_cellSize, const int p_partitionSideLength)

	:gridHeight(p_windowHeight / p_cellSize), gridWidth(p_windowWidth / p_cellSize), cellSize(p_cellSize), 
	partitionSideLength(p_partitionSideLength), partitionWidth(gridWidth / partitionSideLength), partitionHeight(gridHeight / partitionSideLength),
 	gridPartitions(utils::createDynamicArray<bool>(partitionHeight, partitionWidth)),
	grid(utils::createDynamicArray<Entity>(gridHeight, gridWidth)),
	currWorldUpdate(SDL_GetTicks()) {

    if (p_windowWidth % p_cellSize != 0 || p_windowHeight % p_cellSize != 0) {
      throw std::invalid_argument("Window dimensions are not divisible by cell size.");
    }
    if (gridHeight % p_partitionSideLength != 0 || gridWidth % p_partitionSideLength != 0) {
      throw std::invalid_argument("Grid dimensions are not divisible by partition side length.");
    }

		utils::initializeDynamicArray(gridPartitions, partitionSideLength, partitionSideLength); 
		initializeGrid();
		//drawGaltonBoard();
}

void SandWorld::initializeGrid() {
	for (int x = 0; x < gridWidth; ++x) {
		for (int y = 0; y < gridHeight; ++y) {
			grid[x][y].setId(CellId::Air, 0);

			//add barrier
			if (x == 0 || x == gridWidth - 1 || y == 0 || y == gridHeight - 1) {
				grid[x][y].setId(CellId::Stone, currWorldUpdate);
			}
		}

		for (int x = 0; x < partitionSideLength; ++x) {
			for (int y = 0; y < partitionSideLength; ++y) {
				gridPartitions[x][y] = true;
			}
		}
	}
}

// *********************************************************************
// Event Handling
// *********************************************************************

void SandWorld::handleEvent(const Uint8* currKeyStates, int p_x, int p_y) {
	const int x = p_x / cellSize;
	const int y = p_y / cellSize;

		if (currKeyStates[SDL_SCANCODE_0])
			drawCircle(x, y, 3, CellId::Air);
		
		if (currKeyStates[SDL_SCANCODE_1])
			drawCircle(x, y, 3, CellId::Stone);
		
		if (currKeyStates[SDL_SCANCODE_2])
			drawCircle(x, y, 3, CellId::Sand);
		
		if (currKeyStates[SDL_SCANCODE_3])
			drawCircle(x, y, 3, CellId::Water);

		if (currKeyStates[SDL_SCANCODE_4])
			drawCircle(x, y, 3, CellId::Fire);

		if (currKeyStates[SDL_SCANCODE_5])
			drawCircle(x, y, 3, CellId::Smoke);

		if (currKeyStates[SDL_SCANCODE_G])
			drawGaltonBoard();
		
		if (currKeyStates[SDL_SCANCODE_ESCAPE])
			initializeGrid();

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

			if (withinRadius) {
				enablePartitionsAround(p_x + dx, p_y + dy);
				cell.setId(p_id, currWorldUpdate);
			}
		}
	}
}

// *********************************************************************
// Updating World
// *********************************************************************

void SandWorld::updateWorld() {
	for (int x = 0; x < partitionSideLength; ++x) {
		for (int y = 0; y < partitionSideLength; ++y) {
			if (!gridPartitions[x][y]) continue;
			gridPartitions[x][y] = false;
			updatePartition(x, y);
		}
	}
	commitSwaps();
}

void SandWorld::updatePartition(int p_x, int p_y) {

	int xi = p_x * partitionWidth;
	int yi = p_y * partitionHeight;

	int xf = (xi + partitionWidth);
	int yf = (yi + partitionHeight);

	currWorldUpdate = (SDL_GetTicks());

	for (int x = xi; x < xf; ++x) {
		for (int y = yi; y < yf; ++y) {

			//delete cell when it reaches the boundry
			if ((x == 0 || x == gridWidth || y == 0 || y == gridHeight - 1) && grid[x][y].getId() > CellId::Stone) {
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
			}
		}
	}
}

void SandWorld::commitSwaps() {
	std::shuffle(swaps.begin(), swaps.end(), utils::getRandomEngine());
	for (const auto& swap : swaps) {
		Entity& cell1 = grid[swap->x1][swap->y1];
		Entity& cell2 = grid[swap->x2][swap->y2];
		bool cellPrevUpdated = cell1.getLastUpdated() == currWorldUpdate || cell2.getLastUpdated() == currWorldUpdate;

		if (cellPrevUpdated) {
			continue;
		}

		cell1.setLastUpdated(currWorldUpdate);
		cell2.setLastUpdated(currWorldUpdate);

		std::swap(grid[swap->x1][swap->y1], grid[swap->x2][swap->y2]);
	}
	swaps.clear();
}

// *********************************************************************
// Updating Cells
// *********************************************************************

void SandWorld::enablePartitionsAround(int x, int y) {
	bool standardArray[10][10];
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if((x / partitionWidth + dx < 0) || (y / partitionHeight + dy < 0)) continue;
			if ((x / partitionWidth + dx > gridWidth) || (y / partitionHeight + dy > gridHeight)) continue;
			gridPartitions[x / partitionWidth + dx][y / partitionHeight + dy] = true;
		}
	}
}

void SandWorld::updateSand(int x, int y) {
	std::unique_ptr<SwapOperation> swap = grid[x][y].update(grid, x, y);
	if (swap) swaps.push_back(std::move(swap));
}

// *********************************************************************
// Fun
// *********************************************************************

void SandWorld::drawGaltonBoard() {

	int radius = 2;
	int margin = gridHeight / 6; 
	int pegSpacing = radius * 3;
	int rows = (gridHeight - 2 * margin) / pegSpacing;

	//get number of rows based on grid size
	while ((rows * pegSpacing) + 2 * margin > gridHeight || (rows * pegSpacing / 2) + 2 * margin > gridWidth) {
		--rows;
	}

	int startX = gridWidth / 2;
	int startY = margin;

	//draw dropper
	int topY = margin / 2;
	for (int x = 0; x < gridWidth; ++x) {
		if (x != startX) {
			grid[x][topY].setId(CellId::Stone, currWorldUpdate);
		}
	}

	//draw pegs
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col <= row; ++col) {
			int pegX = startX + col * pegSpacing - row * (pegSpacing / 2);
			int pegY = startY + row * pegSpacing;
			if (pegX >= margin && pegX < gridWidth - margin && pegY >= margin && pegY < gridHeight - margin) {
				drawCircle(pegX, pegY, radius, CellId::Stone);
			}
		}
	}

	//draw collectors
	int collectorY = gridHeight;
	for (int col = 0; col <= rows; ++col) {
		int collectorX = startX - (rows * pegSpacing / 2) + col * pegSpacing;
		if (collectorX >= margin && collectorX < gridWidth - margin) {
			for (int y = collectorY; y >= gridHeight - margin; --y) {
				grid[collectorX][y].setId(CellId::Stone, currWorldUpdate);
			}
		}
	}

	//enable all partitions
	for (int x = 0; x < partitionSideLength; ++x) {
		for (int y = 0; y < partitionSideLength; ++y) {
			gridPartitions[x][y] = true;
		}
	}
}
