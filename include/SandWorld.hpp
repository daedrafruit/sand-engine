#pragma once
#include <SDL3/SDL.h>
#include <mutex>
#include <vector>

#include "Entity.hpp"

enum class Event {
	leftMouse,
	rightMouse
};

class partition {
	private:
		bool status = false;
		int lastUpdated = 0;

	public:
		bool isEnabled() const {
			return status;
		}

		int getLastUpdated() const {
			return lastUpdated;
		}

		void setStatus(bool p_status, int p_worldUpdate) {
			status = p_status;
			lastUpdated = p_worldUpdate;
		}

};

class SandWorld {
public:
	SandWorld(const int windowHeight, const int windowWidth, const int p_cellSize, const int p_partitionSizeInCells);

	~SandWorld() {}

	void setWorldUpdate();

	// Responsible for input and other events
	void handleEvent(const bool* currKeyStates, int p_x, int p_y);

	// Draws a filled circle of some id around the passed in coordinates
	void drawCircle(CellId id, int x, int y, int radius);

	//call resepctive update function for each cell in given partition
	std::vector<SwapOp> updatePartition(int x, int y);

	// Call update partition function on each partition in the world
	void updateWorld();

	// When an 'updateCell' function is called, the updates do not happen immediately, 
	// they are added to a list of updates. This functiont randomizes, and applies those updates
	void commitSwaps();

// Sets all adjacent partitions to true
  void enablePartitionsAround(int x, int y);

	inline int getCellSize() const { return cellSize; }
	inline int getGridWidth() const { return gridWidth; }
	inline int getGridHeight() const { return gridHeight; }

	inline const Entity& getCellAt(int x, int y) const { return grid[x][y]; }

	inline int getPartitionSizeInCells() const { return partitionSizeInCells; }
	inline int getNumPartitionsX() const { return numPartitionsX; }
	inline int getNumPartitionsY() const { return numPartitionsY; }
	inline const std::vector<std::vector<partition>>& getWorldPartitions() const { return worldPartitions; }
	inline bool partitionActive(int x, int y) const { return worldPartitions[x][y].isEnabled(); }

private:
	const int gridHeight, gridWidth, cellSize;
	int currWorldUpdate;
	const int partitionSizeInCells, numPartitionsX, numPartitionsY;

	std::vector<std::vector<Entity>> grid;

	void initializeGrid();
	std::vector<SwapOp> updatePartitionsInRange(int xi, int xf, int yi, int yf); 
	
	std::vector<std::vector<partition>> worldPartitions;

	std::vector<std::vector<SwapOp>> swapLists;
	std::mutex mut;

};



