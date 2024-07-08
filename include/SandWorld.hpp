#pragma once
#include <SDL.h>
#include <vector>

#include "Entity.hpp"

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
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int cellSize, const int p_partitionSideLength);

	// Draws layer of 'stone' (id=1) along the perimeter of the world
	void drawBarrier();

	// Responsible for input and other events
	void handleEvent(Event p_event, int p_x, int p_y);

	// Draws a filled circle of some id around the passed in coordinates
	void drawCircle(int p_x, int p_y, int radius, CellId p_id);

	void updatePartition(int x, int y);

	// Calls the respective 'updateCell' function for each cell in the world
	void updateWorld();

	/* When an 'updateCell' function is called, the updates do not happen immediately, 
	 * they are added to a list of updates, this functiont randomizes, and applies those updates */
	void commitSwaps();

  void enablePartitionsAround(int x, int y);

	void updateSand(int x, int y);
	void updateWater(int x, int y);

	inline int getCellSize() const { return cellSize; }
	inline int getGridWidth() const { return gridWidth; }
	inline int getGridHeight() const { return gridHeight; }
	inline const std::vector<std::vector<Entity>>& getGrid() const { return grid; }

	inline int getPartitionSideLength() const { return partitionSideLength; }
	inline int getPartitionWidth() const { return partitionWidth; }
	inline int getPartitionHeight() const { return partitionHeight; }
	inline const std::vector<std::vector<bool>>& getGridPartitions() const { return gridPartitions; }

private:
	const int gridHeight, gridWidth, cellSize;
	int currWorldUpdate;
	const int partitionSideLength, partitionWidth, partitionHeight;

	std::vector<std::vector<Entity>> grid;
	std::vector<SwapOperation> swaps;

	std::vector<std::vector<bool>> gridPartitions;
};



