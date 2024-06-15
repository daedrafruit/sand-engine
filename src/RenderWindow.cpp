
#include <iostream>
#include <SDl.h>

#include "entity.hpp"
#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL), width(p_w), height(p_h) {

	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		std::cout << "Window failed to init" << SDL_GetError() << std::endl; 
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
} 

void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity, const int p_x, const int p_y, const int& p_cellSize) {

	SDL_Rect rect = { p_x, p_y, p_cellSize, p_cellSize};

	Color color = p_entity.getColor();

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

const int RenderWindow::getWidth() const {
	return width;
}

const int RenderWindow::getHeight() const {
	return height;
}
