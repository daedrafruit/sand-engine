
#pragma once
#include <SDl.h>

class RenderWindow {

public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	void cleanUp();
	void clear();
	void drawPixel(int p_x, int p_y, Uint8 p_r, Uint8 p_g, Uint8 p_b);
	void display();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};