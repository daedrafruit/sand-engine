#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>
#include <future>
#include <vector>
#include <SDL3/SDL.h>
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

    if (windowWidth % p_cellSize != 0 || windowHeight % p_cellSize != 0)
      throw std::invalid_argument("Window dimensions are not divisible by cell size.");

    if (gridHeight % p_partitionSizeInCells != 0 || gridWidth % p_partitionSizeInCells != 0)
      throw std::invalid_argument("Grid dimensions are not divisible by partition side length.");

		int threads = 10;
		if ((numPartitionsX + numPartitionsY) % threads != 0) 
      throw std::invalid_argument("Number of partitions is not divisible by thread count.");


		initializeGrid();
}

void SandWorld::initializeGrid() {

	grid.resize(gridWidth);
	for (int x = 0; x < gridWidth; ++x) {
			grid[x].resize(gridHeight);
	}

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			grid[x][y] = Entity(currWorldUpdate, CellId::Air);

			//add barrier
			if (x == 0 || x == gridWidth - 1 || y == 0 || y == gridHeight - 1) {
				grid[x][y] = Entity(currWorldUpdate, CellId::Stone);
			}
		}
	}

	for (int y = 0; y < numPartitionsY; ++y) {
		for (int x = 0; x < numPartitionsX; ++x) {
			worldPartitions[x][y].setStatus(true, currWorldUpdate);
		}
	}
}

// *********************************************************************
// Event Handling
// *********************************************************************

void SandWorld::handleEvent(const bool* currKeyStates, int p_x, int p_y) {
	const int x = p_x / cellSize;
	const int y = p_y / cellSize;

	if (currKeyStates[SDL_SCANCODE_0])
		drawCircle(CellId::Air, x, y, 3);
	
	if (currKeyStates[SDL_SCANCODE_1])
		drawCircle(CellId::Stone, x, y, 3);
	
	if (currKeyStates[SDL_SCANCODE_2])
		drawCircle(CellId::Sand, x, y, 3);
	
	if (currKeyStates[SDL_SCANCODE_3])
		drawCircle(CellId::Water, x, y, 3);

	if (currKeyStates[SDL_SCANCODE_4])
		drawCircle(CellId::Fire, x, y, 3);

	if (currKeyStates[SDL_SCANCODE_5])
		drawCircle(CellId::Smoke, x, y, 3);

	if (currKeyStates[SDL_SCANCODE_ESCAPE])
		initializeGrid();
}

void SandWorld::drawCircle(CellId id, int x, int y, int radius) {
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
				grid[cellx][celly].setId(id, 0);
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
	int sizeX = numPartitionsX / 4;
	int sizeY = numPartitionsY / 4;

	for (int iterationY = 0; iterationY < 2; ++iterationY) {
		for (int iterationX = 0; iterationX < 2; ++iterationX) {

			int interationStartX = sizeX * iterationX;
			int iterationEndX = sizeX * (iterationX+1);

			int iterationStartY = sizeY * iterationY;
			int iterationEndY = sizeY * (iterationY+1);

			std::vector<std::future<std::vector<SwapOp>>> futures;
			for (int halfY = 0; halfY < 2; ++halfY) {
				for (int halfX = 0; halfX < 2; ++halfX) {

					int currStartX = interationStartX + (numPartitionsX/2 * halfX);
					int currEndX   = iterationEndX    + (numPartitionsX/2 * halfX);

					int currStartY = iterationStartY  + (numPartitionsY/2 * halfY);
					int currEndY   = iterationEndY    + (numPartitionsY/2 * halfY);

					futures.emplace_back(std::async(std::launch::async, &SandWorld::updatePartitionsInRange, this, currStartX, currEndX, currStartY, currEndY));

				}
			}
			for (std::future<std::vector<SwapOp>>& f : futures) {
				for (SwapOp& s : f.get()) {
					worldSwaps.emplace_back(s);
				}
			}
		}
	}
	commitSwaps();
}

std::vector<SwapOp> SandWorld::updatePartitionsInRange(int xi, int xf, int yi, int yf) {
	std::vector<SwapOp> swaps;
	for (int x = xi; x < xf; ++x) {
		for (int y = yi; y < yf; ++y) {
			if (!worldPartitions[x][y].isEnabled()) continue;

			partition& currPartition = worldPartitions[x][y];

			if (currPartition.getLastUpdated() != currWorldUpdate) {
				currPartition.setStatus(false, currWorldUpdate);
			}
			updatePartition(x, y, swaps); 
		}
	}
	return swaps;
}


void SandWorld::updatePartition(int p_x, int p_y, std::vector<SwapOp>& swaps) {

	int xi = p_x * partitionSizeInCells;
	int yi = p_y * partitionSizeInCells;

	int xf = (xi + partitionSizeInCells);
	int yf = (yi + partitionSizeInCells);

	for (int y = yi; y < yf; ++y) {
		for (int x = xi; x < xf; ++x) {

			Entity& currCell = grid[x][y];
			if (currCell.getId() == CellId::Air) continue;
			if (currCell.getId() == CellId::Stone) continue;

			bool atEdge = (x <= 0 || x >= gridWidth - 1) ||
			              (y <= 0 || y >= gridHeight - 1);

			if (atEdge && currCell.getId() > CellId::Stone) {
				grid[x][y].setId(CellId::Air, currWorldUpdate);
				continue;
			} 

			int oldLen = swaps.size();

			currCell.update(grid, x, y, swaps);

			bool areNewSwaps = swaps.size() > oldLen;
			if (areNewSwaps) enablePartitionsAround(x, y);	
		}
	}
}

void SandWorld::commitSwaps() {
	std::shuffle(worldSwaps.begin(), worldSwaps.end(), utils::getRandomEngine());
	for (SwapOp& swap : worldSwaps) {

		Entity& cell1 = grid[swap.x1][swap.y1];
		Entity& cell2 = grid[swap.x2][swap.y2];

		//if swap contains entity, it is a replace swap
		if (swap.id != CellId::Null) {
			cell1.setId(swap.id, currWorldUpdate);
			continue;
		}

		bool cellPrevUpdated = cell1.lastUpdated == currWorldUpdate || cell2.lastUpdated == currWorldUpdate;

		if (cellPrevUpdated) {
			continue;
		}

		cell1.lastUpdated = currWorldUpdate;
		cell2.lastUpdated = currWorldUpdate;

		std::swap(cell1, cell2);
	}
	worldSwaps.clear();
}

void SandWorld::enablePartitionsAround(int x, int y) {
	int partitionX = x / partitionSizeInCells;
	int partitionY = y / partitionSizeInCells;
	int radius = 1;

	for (int dy = -radius; dy <= radius; ++dy) {
		for (int dx = -radius; dx <= radius; ++dx) {
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

