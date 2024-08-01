#pragma once
#include <SDL.h>
#include <memory>
#include <vector>

#include "Entity.hpp"

enum class Event {
	leftMouse,
	rightMouse
};

class SandWorld {
public:
	SandWorld(const int windowHeight, const int windowWidth, const int p_cellSize, const int p_partitionSideLength);

	~SandWorld() {}

	void drawGaltonBoard();

	// Responsible for input and other events
	void handleEvent(const Uint8* currKeyStates, int p_x, int p_y);

	// Draws a filled circle of some id around the passed in coordinates
	template <typename T>
	void drawCircle(int x, int y, int radius);

	void updatePartition(int x, int y);

	// Calls the respective 'updateCell' function for each cell in the world
	void updateWorld();

	// When an 'updateCell' function is called, the updates do not happen immediately, 
	// they are added to a list of updates. This functiont randomizes, and applies those updates
	void commitSwaps();

	// Sets all adjacent partitions to true
  void enablePartitionsAround(int x, int y);

	inline int getCellSize() const { return cellSize; }
	inline int getGridWidth() const { return gridWidth; }
	inline int getGridHeight() const { return gridHeight; }

	inline const std::unique_ptr<Entity>& getCellAt(int x, int y) const { return grid[x][y]; }

	inline int getPartitionSideLength() const { return partitionSideLength; }
	inline int getPartitionWidth() const { return partitionWidth; }
	inline int getPartitionHeight() const { return partitionHeight; }
	inline std::vector<std::vector<bool>> getWorldPartitions() const { return worldPartitions; }
	inline bool partitionActive(int x, int y) const { return worldPartitions[x][y]; }

private:
	const int gridHeight, gridWidth, cellSize;
	int currWorldUpdate;
	const int partitionSideLength, partitionWidth, partitionHeight;

	std::vector<std::vector<std::unique_ptr<Entity>>> grid;

	void initializeGrid();
	
	std::vector<std::vector<bool>> worldPartitions;

	std::vector<SwapOp> swaps;

};



