#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>
#include <thread>
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

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = 0; y < gridHeight; ++y) {
			grid[x][y] = Entity(currWorldUpdate, CellId::Air);

			//add barrier
			if (x == 0 || x == gridWidth - 1 || y == 0 || y == gridHeight - 1) {
				grid[x][y] = Entity(currWorldUpdate, CellId::Stone);
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

void SandWorld::updatePartitionRange(int xi, int xf, int yi, int yf) {
	for (int x = xi; x < xf; ++x) {
		for (int y = yi; y < yf; ++y) {
			if (!worldPartitions[x][y].isEnabled()) continue;

			partition& currPartition = worldPartitions[x][y];

			if (currPartition.getLastUpdated() != currWorldUpdate) {
				currPartition.setStatus(false, currWorldUpdate);
			}
			updatePartition(x, y);
		}
	}
}

void SandWorld::updateWorld() {
	int num_threads = 4;
	int size_x = numPartitionsX / num_threads;
	int size_y = numPartitionsY / num_threads;

	for (int iteration_y = 0; iteration_y < 2; ++iteration_y) {
		for (int iteration_x = 0; iteration_x < 2; ++iteration_x) {

			int interation_x_start = size_x * iteration_x;
			int iteration_x_end = size_x * (iteration_x+1);

			int iteration_y_start = size_y * iteration_y;
			int iteration_y_end = size_y * (iteration_y+1);

			std::vector<std::thread> threads;
			for (int half_y = 0; half_y < 2; ++half_y) {
				for (int half_x = 0; half_x < 2; ++half_x) {

					int curr_x_start = interation_x_start + (numPartitionsX/2 * half_x);
					int curr_x_end   = iteration_x_end    + (numPartitionsX/2 * half_x);

					int curr_y_start = iteration_y_start  + (numPartitionsY/2 * half_y);
					int curr_y_end   = iteration_y_end    + (numPartitionsY/2 * half_y);

						threads.push_back(std::thread(&SandWorld::updatePartitionRange, this, curr_x_start, curr_x_end, curr_y_start, curr_y_end));
				}
			}
			for (auto& t : threads) {
				t.join();
			}
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

			Entity& currCell = grid[x][y];
			if (currCell.getId() == CellId::Air) continue;
			if (currCell.getId() == CellId::Stone) continue;

			bool atEdge = (x <= 0 || x >= gridWidth - 1) ||
			              (y <= 0 || y >= gridHeight - 1);

			if (atEdge && currCell.getId() > CellId::Stone) {
				grid[x][y].setId(CellId::Air, currWorldUpdate);
				continue;
			} 

			std::vector<SwapOp> newSwaps = currCell.update(grid, x, y);
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

