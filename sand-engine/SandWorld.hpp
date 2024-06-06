#pragma once
#include <SDL.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"

class SandWorld {
public:
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int cellSize);

	void updateWorld();
	void drawBarrier();
	void renderWorld(RenderWindow p_window);
	void mouseEvent(RenderWindow p_window);

private:
	const int gridHeight, gridWidth, cellSize;
    std::vector<Entity> entities;

	bool currWorldUpdate;
};



