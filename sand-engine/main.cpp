
#include <iostream>
#include <SDL.h>
#include <vector>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    const float cellSize = 5;
    const int windowWidth = 1280;
    const int windowHeight = 720;

    RenderWindow window("Falling Sand", windowWidth, windowHeight);

    const int gridHeight = windowHeight / cellSize;
    const int gridWidth = windowWidth / cellSize;
    std::vector<std::vector<Entity>> grid(gridWidth, std::vector<Entity>(gridHeight, Entity(0)));
    std::vector<std::vector<Entity>> bufferGrid(gridWidth, std::vector<Entity>(gridHeight, Entity(0)));

    bool gameRunning = true;
    SDL_Event event;
    
    const float timeStep = 0.05f;
    const float maxTimeStep = 0.25f;
    float accumulator = 0.0f;
    float prevTime = utils::hireTimeInSeconds();

    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                gameRunning = false;
        }
        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX < windowWidth - 1 && mouseX > 1 && mouseY < windowHeight - 1 && mouseY > 1) {
			if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						grid[mouseX/cellSize][mouseY/cellSize].setId(1);
						grid[mouseX/cellSize + 1][mouseY/cellSize].setId(1);
						grid[mouseX/cellSize - 1][mouseY/cellSize].setId(1);
						grid[mouseX/cellSize][mouseY/cellSize + 1].setId(1);
						grid[mouseX/cellSize][mouseY/cellSize - 1].setId(1);
			}

			if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
						grid[mouseX/cellSize][mouseY/cellSize].setId(0);
						grid[mouseX/cellSize + 1][mouseY/cellSize].setId(0);
						grid[mouseX/cellSize - 1][mouseY/cellSize].setId(0);
						grid[mouseX/cellSize][mouseY/cellSize + 1].setId(0);
						grid[mouseX/cellSize][mouseY/cellSize - 1].setId(0);
			}
        }

        float newTime = utils::hireTimeInSeconds();
        float deltaTime = newTime - prevTime;
        prevTime = newTime;

        accumulator += std::min(deltaTime, maxTimeStep);


        while (accumulator >= timeStep) {
            
            utils::copyEntityGrid(grid, bufferGrid);

			for (int x = 0; x < gridWidth; ++x) {
				for (int y = 0; y < gridHeight - 1; ++y) {
					if (!grid[x][y].isEmpty()) {
						if (grid[x][y+1].isEmpty()) {
							bufferGrid[x][y].setId(0);
							bufferGrid[x][y+1].setId(1);
						} else if (x > 0 && grid[x-1][y+1].isEmpty()) {
							bufferGrid[x][y].setId(0);
							bufferGrid[x-1][y+1].setId(1);
						} else if (x < gridWidth - 1 && grid[x+1][y+1].isEmpty()) {
							bufferGrid[x][y].setId(0);
							bufferGrid[x+1][y+1].setId(1);
						}
                    }
				}
			}
            utils::copyEntityGrid(bufferGrid, grid);
            accumulator -= timeStep;
        }

        window.clear();

        for (int x = 0; x < gridWidth; x++) {
            for (int y = 0; y < gridHeight; y++) {
                const int gridX = x * cellSize;
                const int gridY = y * cellSize;

                window.render(grid[x][y], gridX, gridY, cellSize);
            }
        }

        window.display();
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
