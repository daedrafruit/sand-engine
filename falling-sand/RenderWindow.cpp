
#include <iostream>
#include <SDl.h>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL) {

	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		std::cout << "Window failed to init" << SDL_GetError() << std::endl; 
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
} 


void RenderWindow::cleanUp() {
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}

void RenderWindow::drawPixel(int p_x, int p_y, Uint8 p_r, Uint8 p_g, Uint8 p_b) {
    SDL_SetRenderDrawColor(renderer, p_r, p_g, p_b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, p_x, p_y);
    SDL_RenderPresent(renderer);

}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}
