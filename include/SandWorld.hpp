#pragma once
#include <SDL.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"

class SandWorld {
public:
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int cellSize);

	void drawBarrier();

	void updateWorld();
	void renderWorld(RenderWindow& p_window);
	void mouseEvent(const RenderWindow& p_window);

private:
	const int gridHeight, gridWidth, cellSize;
    std::vector<std::vector<Entity>> grid;

	bool currWorldUpdate;
};



