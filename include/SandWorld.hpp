#pragma once
#include <SDL.h>
#include <vector>

#include "RenderWindow.hpp"

struct SwapOperation {
    int x1, y1;
    int x2, y2;
};

class SandWorld {
public:
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int cellSize);

	void drawBarrier();

	void updateWorld();
	void renderWorld(RenderWindow& p_window);
	void mouseEvent(const RenderWindow& p_window);

	void updateSand(int x, int y);
	void updateWater(int x, int y);

	void updateCells(std::vector<SwapOperation> p_checkCells);

	void commitSwaps();

private:
	const int gridHeight, gridWidth, cellSize;
    std::vector<std::vector<Entity>> grid;

	int currWorldUpdate;
	std::vector<SwapOperation> swaps;
};



