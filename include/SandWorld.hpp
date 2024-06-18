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
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int cellSize);

	void drawBarrier();

	void handleEvent(Event p_event, int p_x, int p_y);

	void drawCircle(int p_x, int p_y, int radius, int p_id);

	void updateWorld();

	void commitSwaps();

	void updateSand(int x, int y);
	void updateWater(int x, int y);

	inline int getCellSize() const { return cellSize; }
	inline int getGridWidth() const { return gridWidth; }
	inline int getGridHeight() const { return gridHeight; }
	inline const std::vector<std::vector<Entity>>& getGrid() const { return grid; }

private:
	const int gridHeight, gridWidth, cellSize;
	int currWorldUpdate;

	std::vector<std::vector<Entity>> grid;
	std::vector<SwapOperation> swaps;
};



