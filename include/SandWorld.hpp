#pragma once
#include <SDL.h>
#include <vector>

#include "RenderWindow.hpp"

class SandWorld {
public:
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int cellSize);

	void drawBarrier();

	void updateWorld();
	void renderWorld(RenderWindow& p_window);
	void mouseEvent(const RenderWindow& p_window);

	void swapCells(Entity& cell1, Entity& cell2);

	void updateSand(int x, int y);
	void updateWater(int x, int y);

private:
	const int gridHeight, gridWidth, cellSize;
    std::vector<std::vector<Entity>> grid;

	bool currWorldUpdate;
};



