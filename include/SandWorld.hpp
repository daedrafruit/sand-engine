#pragma once
#include <SDL.h>
#include <vector>

#include "Entity.hpp"
#include "utils.hpp"

struct SwapOperation {
	int x1, y1;
	int x2, y2;
};

enum class Event {
	leftMouse,
	rightMouse
};

class SandWorld {
public:
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int p_cellSize, const int p_partitionSideLength);

	~SandWorld() {
		utils::deleteDynamicArray(grid, gridHeight);
		utils::deleteDynamicArray(gridPartitions, partitionHeight);
	}

	void drawGaltonBoard();

	// Responsible for input and other events
	void handleEvent(const Uint8* currKeyStates, int p_x, int p_y);

	// Draws a filled circle of some id around the passed in coordinates
	void drawCircle(int p_x, int p_y, int radius, CellId p_id);

	void updatePartition(int x, int y);

	// Calls the respective 'updateCell' function for each cell in the world
	void updateWorld();

	// When an 'updateCell' function is called, the updates do not happen immediately, 
	// they are added to a list of updates. This functiont randomizes, and applies those updates
	void commitSwaps();

	// Sets all adjacent partitions to true
  void enablePartitionsAround(int x, int y);

	// Updates sand cell, also respoinsible for enabling respective partitions
	void updateSand(int x, int y);

	// Updates water cell, also respoinsible for enabling respective partitions
	void updateWater(int x, int y);

	inline int getCellSize() const { return cellSize; }
	inline int getGridWidth() const { return gridWidth; }
	inline int getGridHeight() const { return gridHeight; }

	inline const Entity& getCellAt(int x, int y) const { return grid[x][y]; }

	inline int getPartitionSideLength() const { return partitionSideLength; }
	inline int getPartitionWidth() const { return partitionWidth; }
	inline int getPartitionHeight() const { return partitionHeight; }
	inline const bool& partitionActive(int x, int y) const { return gridPartitions[x][y]; }

private:
	const int gridHeight, gridWidth, cellSize;
	int currWorldUpdate;
	const int partitionSideLength, partitionWidth, partitionHeight;

	Entity** grid;
	void initializeGrid();
	
	bool** gridPartitions;

	std::vector<SwapOperation> swaps;

};



