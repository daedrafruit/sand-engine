#pragma once
#include <SDL.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"

class SandWorld {
public:
	SandWorld(const int p_windowHeight, const int p_windowWidth, const int cellSize);

	void updateWorld();
	void renderWorld(RenderWindow p_window);
	void mouseEvent(RenderWindow p_window);

	void copyGrid(const std::vector<std::vector<Entity>>& source, std::vector<std::vector<Entity>>& destination);

private:
	const int gridHeight, gridWidth, cellSize;
    std::vector<std::vector<Entity>> grid;
    std::vector<std::vector<Entity>> bufferGrid;
};



