
#pragma once
#include <SDl.h>
#include <memory>

#include "Entity.hpp"
#include "SandWorld.hpp"

class RenderWindow {

public:
	RenderWindow(const char* p_title, const int p_w, const int p_h);

	const int getWidth() const;
	const int getHeight() const;

	void cleanUp();
	void clear();
	void render(const std::unique_ptr<Entity>& p_entity, int p_x, int p_y, const int& p_cellSize);
	void display();

	void renderWorld(const SandWorld& p_world);
	void renderPartition(int p_x, int p_y, const SandWorld& p_world);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	const int width, height;
};
