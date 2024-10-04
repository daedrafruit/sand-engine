
#include <iostream>
#include <SDL2/SDL.h>

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_mouse.h"
#include "Utils.hpp"
#include "RenderWindow.hpp"
#include "SandWorld.hpp"

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int cellSize = 2;
		const int partitionSideLength = 20;

    RenderWindow window("Falling Sand", windowWidth, windowHeight, partitionSideLength);

    SandWorld world(windowHeight, windowWidth, cellSize, partitionSideLength);

		window.updateRenderPartitions(world.getWorldPartitions());

    bool gameRunning = true;
    SDL_Event event;
    
    const float timeStep = 0.01f;
    const float maxTimeStep = 0.25f;
    float accumulator = 0.0f;
    float prevTime = utils::hireTimeInSeconds();

    while (gameRunning) {

			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) 
					gameRunning = false;
			}

			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			int x, y;
			SDL_GetMouseState(&x, &y);

			float newTime = utils::hireTimeInSeconds();
			float deltaTime = newTime - prevTime;
			prevTime = newTime;

			accumulator += std::min(deltaTime, maxTimeStep);

			while (accumulator >= timeStep) {
				world.setWorldUpdate();
				world.handleEvent(currentKeyStates, x, y);
				world.updateWorld();
				window.updateRenderPartitions(world.getWorldPartitions());
				accumulator -= timeStep;
			}

			//window.clear();

			window.renderWorld(world);

			window.display();
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}


