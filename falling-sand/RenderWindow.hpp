
#pragma once
#include <SDl.h>
#include "Entity.hpp"

class RenderWindow 
{

public:
	RenderWindow(const char* p_title, const int p_w, const int p_h);

	void cleanUp();
	void clear();
	void render(Entity& p_entity, int p_x, int p_y, const float& p_cellSize);
	void display();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};