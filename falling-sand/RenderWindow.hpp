
#pragma once
#include <SDl.h>
#include "Entity.hpp"

class RenderWindow 
{

public:
	RenderWindow(const char* p_title, int p_w, int p_h);

	void cleanUp();
	void clear();
	void render(Entity& p_entity, int& p_cellSize);
	void display();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};