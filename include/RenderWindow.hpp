
#pragma once
#include <SDl.h>
#include <memory>
#include <vector>

#include "Entity.hpp"
#include "SandWorld.hpp"

class RenderWindow {

public:
	RenderWindow(const char* title, const int p_width, const int p_height, int partitionSideLength);

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height;	}

	void cleanUp();
	void clear();
	void render(const std::unique_ptr<Entity>& entity, int x, int y, int cellSize);
	void display();

	void renderWorld(const SandWorld& world);
	void renderPartition(int p_x, int p_y, const SandWorld& world);
	void updateRenderPartitions(std::vector<std::vector<bool>> worldPartitions);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::vector<std::vector<bool>> renderPartitions;
	const int width, height;
	const int partitionSideLength;
};
