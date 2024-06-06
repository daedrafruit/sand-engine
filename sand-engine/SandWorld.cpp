#include <vector>
#include <SDL.h>
#include <iostream>

#include "RenderWindow.hpp"
#include "SandWorld.hpp"
#include "Entity.hpp"


SandWorld::SandWorld(const int p_windowHeight, const int p_windowWidth, const int p_cellSize)
	:gridHeight(p_windowHeight / p_cellSize), gridWidth(p_windowWidth / p_cellSize), cellSize(p_cellSize),
	 currWorldUpdate(false) {
}

void SandWorld::renderWorld(RenderWindow p_window) {
	for (Entity& e : entities) {
		p_window.render(e, cellSize);
	}
}

void SandWorld::updateWorld() {

	currWorldUpdate = (!currWorldUpdate);

}

void SandWorld::mouseEvent(RenderWindow p_window) {
	int mouseX, mouseY;
	Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	int radius = 5;

	if (mouseX < p_window.getWidth() - 5 && mouseX > 5 && mouseY < p_window.getHeight() - 5 && mouseY > 5) {
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			entities.push_back(Entity(mouseX, mouseY, 1));
		}

		if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		}
	}
}

void SandWorld::copyGrid(const std::vector<std::vector<Entity>>& source, std::vector<std::vector<Entity>>& destination) {
	for (size_t i = 0; i < source.size(); ++i) {
		for (size_t j = 0; j < source[i].size(); ++j) {
			destination[i][j] = source[i][j];
		}
	}
}
