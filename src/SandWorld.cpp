#include <algorithm>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <stdexcept>

#include "SandWorld.hpp"
#include "Entity.hpp"
#include "SDL_scancode.h"
#include "SDL_timer.h"
#include "Utils.hpp"

// *********************************************************************
// Initialization
// *********************************************************************

SandWorld::SandWorld(const int windowHeight, const int windowWidth, const int p_cellSize, const int p_partitionSizeInCells)
	:gridHeight(windowHeight / p_cellSize), gridWidth(windowWidth / p_cellSize), cellSize(p_cellSize), 
	partitionSizeInCells(p_partitionSizeInCells),
	numPartitionsX(gridWidth / partitionSizeInCells),
	numPartitionsY(gridHeight / partitionSizeInCells),
	worldPartitions(numPartitionsX, std::vector<partition>(numPartitionsY)),
	currWorldUpdate(SDL_GetTicks()) {

    if (windowWidth % p_cellSize != 0 || windowHeight % p_cellSize != 0) {
      throw std::invalid_argument("Window dimensions are not divisible by cell size.");
    }
    if (gridHeight % p_partitionSizeInCells != 0 || gridWidth % p_partitionSizeInCells != 0) {
      throw std::invalid_argument("Grid dimensions are not divisible by partition side length.");
    }

		initializeGrid();
		//drawGaltonBoard();
}

void SandWorld::initializeGrid() {

	grid.resize(gridWidth);
	for (int x = 0; x < gridWidth; ++x) {
			grid[x].resize(gridHeight);
	}

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = 0; y < gridHeight; ++y) {
			grid[x][y] = std::make_unique<Air>(currWorldUpdate);

			//add barrier
			if (x == 0 || x == gridWidth - 1 || y == 0 || y == gridHeight - 1) {
				grid[x][y] = std::make_unique<Stone>(currWorldUpdate);
			}
		}
	}

	for (int x = 0; x < numPartitionsX; ++x) {
		for (int y = 0; y < numPartitionsY; ++y) {
			worldPartitions[x][y].setStatus(true, currWorldUpdate);
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
		drawCircle<Air>(x, y, 3);
	
	if (currKeyStates[SDL_SCANCODE_1])
		drawCircle<Stone>(x, y, 3);
	
	if (currKeyStates[SDL_SCANCODE_2])
		drawCircle<Sand>(x, y, 3);
	
	if (currKeyStates[SDL_SCANCODE_3])
		drawCircle<Water>(x, y, 3);

	if (currKeyStates[SDL_SCANCODE_4])
		drawCircle<Fire>(x, y, 3);

	if (currKeyStates[SDL_SCANCODE_5])
		drawCircle<Smoke>(x, y, 3);

	if (currKeyStates[SDL_SCANCODE_G])
		drawGaltonBoard();
	
	if (currKeyStates[SDL_SCANCODE_ESCAPE])
		initializeGrid();
}

template <typename T>
void SandWorld::drawCircle(int x, int y, int radius) {

	for (int dx = -radius; dx <= radius; ++dx) {
		for (int dy = -radius; dy <= radius; dy++) {
			int cellx = x + dx;
			int celly = y + dy;

			// Quadratic formula
			bool withinRadius = dx * dx + dy * dy <= radius * radius;
			bool withinBounds = (cellx >= 0 && cellx < gridWidth) &&
													(celly >= 0 && celly < gridHeight);

			if (withinRadius && withinBounds) {
				enablePartitionsAround(cellx, celly);
				std::unique_ptr<Entity> cell = std::make_unique<T>(currWorldUpdate);
				swaps.emplace_back(cellx, celly, cellx, celly, std::move(cell));
			}
		}
	}
}

// *********************************************************************
// Updating World
// *********************************************************************

void SandWorld::setWorldUpdate() {
	currWorldUpdate = (SDL_GetTicks());
}

void SandWorld::updateWorld() {

	for (int x = 0; x < numPartitionsX; ++x) {
		for (int y = 0; y < numPartitionsY; ++y) {
			if (!worldPartitions[x][y].isEnabled()) continue;

			partition& currPartition = worldPartitions[x][y];

			if (currPartition.getLastUpdated() != currWorldUpdate) {
				currPartition.setStatus(false, currWorldUpdate);
			}
			updatePartition(x, y);
		}
	}
	commitSwaps();
}

void SandWorld::updatePartition(int p_x, int p_y) {

	int xi = p_x * partitionSizeInCells;
	int yi = p_y * partitionSizeInCells;

	int xf = (xi + partitionSizeInCells);
	int yf = (yi + partitionSizeInCells);

	for (int x = xi; x < xf; ++x) {
		for (int y = yi; y < yf; ++y) {

			std::unique_ptr<Entity>& currCell = grid[x][y];

			bool atEdge = (x <= 0 || x >= gridWidth - 1) ||
			              (y <= 0 || y >= gridHeight - 1);

			if (atEdge && currCell->getId() > CellId::Stone) {
				std::unique_ptr<Entity> air = std::make_unique<Air>(0);
				swaps.emplace_back(x, y, x, y, std::move(air));
				continue;
			} 

			std::vector<SwapOp> newSwaps = currCell->update(grid, x, y);
			if (!newSwaps.empty()) {
				enablePartitionsAround(x, y);
				for (SwapOp& swap : newSwaps) {
					swaps.emplace_back(std::move(swap));
				}
			}
		}
	}
}

void SandWorld::commitSwaps() {
	std::shuffle(swaps.begin(), swaps.end(), utils::getRandomEngine());
	for (SwapOp& swap : swaps) {

		std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		//if swap contains entity, it is a replace swap
		if (swap.newEntity) {
			cell1 = std::move(swap.newEntity);
			cell1->setLastUpdated(currWorldUpdate);
			continue;
		}

		bool cellPrevUpdated = cell1->getLastUpdated() == currWorldUpdate || cell2->getLastUpdated() == currWorldUpdate;

		if (cellPrevUpdated) {
			continue;
		}

		cell1->setLastUpdated(currWorldUpdate);
		cell2->setLastUpdated(currWorldUpdate);

		std::swap(cell1, cell2);
	}
	swaps.clear();
}

void SandWorld::enablePartitionsAround(int x, int y) {
	int partitionX = x / partitionSizeInCells;
	int partitionY = y / partitionSizeInCells;
	int radius = 1;

	for (int dx = -radius; dx <= radius; ++dx) {
		for (int dy = -radius; dy <= radius; ++dy) {
			int newPartitionX = partitionX + dx;
			int newPartitionY = partitionY + dy;

			bool withinBounds = (newPartitionX >= 0 && newPartitionX < numPartitionsX) &&
			                   	(newPartitionY >= 0 && newPartitionY < numPartitionsY);

			if (withinBounds) {
				partition& currPartition = worldPartitions[newPartitionX][newPartitionY];
				currPartition.setStatus(true, currWorldUpdate);
			}
		}
	}
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
			grid[x][topY] = std::make_unique<Stone>(currWorldUpdate);
		}
	}

	//draw pegs
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col <= row; ++col) {
			int pegX = startX + col * pegSpacing - row * (pegSpacing / 2);
			int pegY = startY + row * pegSpacing;
			if (pegX >= margin && pegX < gridWidth - margin && pegY >= margin && pegY < gridHeight - margin) {
				drawCircle<Stone>(pegX, pegY, radius);
			}
		}
	}

	//draw collectors
	int collectorY = gridHeight;
	for (int col = 0; col <= rows; ++col) {
		int collectorX = startX - (rows * pegSpacing / 2) + col * pegSpacing;
		if (collectorX >= margin && collectorX < gridWidth - margin) {
			for (int y = collectorY; y >= gridHeight - margin; --y) {
				grid[collectorX][y] = std::make_unique<Stone>(currWorldUpdate);
			}
		}
	}

	//enable all partitions
	for (int x = 0; x < numPartitionsX; ++x) {
		for (int y = 0; y < numPartitionsY; ++y) {
			worldPartitions[x][y].setStatus(true, currWorldUpdate);
		}
	}
}
