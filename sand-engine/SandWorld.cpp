#include <vector>
#include <SDL.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "SandWorld.hpp"
#include "Entity.hpp"


SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int p_cellSize)
	:gridHeight(p_windowHeight / p_cellSize), gridWidth(p_windowWidth / p_cellSize), cellSize(p_cellSize),
	 currWorldUpdate(false) {
	drawBarrier();
}

void SandWorld::renderWorld(RenderWindow p_window) {
	for (Entity& e : entities) {
		p_window.render(e, cellSize);
	}
}

void SandWorld::drawBarrier() {
	std::cout << "drawing barrier" << std::endl;
	int left = 0;
	int right = (gridWidth * cellSize);
	int top = 0;
	int bottom = (gridHeight * cellSize);

	//left and right sides, also corners
	for (int y = 0; y < bottom; y += cellSize) {
		entities.push_back(Entity(left, y, 0, cellSize));
		entities.push_back(Entity(right - cellSize, y, 0, cellSize));
	}
	//bottom and top, without corners
	for (int x = cellSize; x < right; x += cellSize) {
		entities.push_back(Entity(x, top, 0, cellSize));
		entities.push_back(Entity(x, bottom - cellSize, 0, cellSize));
	}
}

void SandWorld::updateWorld() {

	for (Entity& e : entities) {
		e.updateCell(entities);
	}

	currWorldUpdate = (!currWorldUpdate);

}

void SandWorld::mouseEvent(RenderWindow p_window) {
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    int radius = 5;

    if (mouseX < p_window.getWidth() - 5 && mouseX > 5 && mouseY < p_window.getHeight() - 5 && mouseY > 5) {
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            // Round mouse coordinates to nearest cell
            int roundedX = (mouseX / cellSize) * cellSize;
            int roundedY = (mouseY / cellSize) * cellSize;

            entities.push_back(Entity(roundedX, roundedY, 1, cellSize));
        }

        if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            // Right mouse button logic
        }
    }
}

