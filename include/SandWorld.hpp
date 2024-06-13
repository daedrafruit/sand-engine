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

	void updateSand(int x, int y);

private:
	const int gridHeight, gridWidth, cellSize;
    std::vector<std::vector<Entity>> grid;

	bool currWorldUpdate;
};



