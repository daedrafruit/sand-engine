#include <SDL3/SDL_init.h>
#include <iostream>
#include <SDL3/SDL.h>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "SDL_keyboard.h"
#include "Utils.hpp"
#include "RenderWindow.hpp"
#include "SandWorld.hpp"

int main(int argc, char* args[]) {
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    const int windowWidth = 800;
    const int windowHeight = 800;
    const int cellSize = 1;
		const int partitionSizeInCells = 10;


		std::cout << "Initializing World... " << std::endl;
    SandWorld world(windowHeight, windowWidth, cellSize, partitionSizeInCells);
		std::cout << "done." << std::endl;

		std::cout << "Initializing Window... " << std::endl;
    RenderWindow window("Falling Sand", windowWidth, windowHeight);
		std::cout << "done." << std::endl;

    bool gameRunning = true;
    SDL_Event event;
    
    const float timeStep = 0.01f;
    const float maxTimeStep = 0.25f;
    float accumulator = 0.0f;
    float prevTime = utils::hireTimeInSeconds();

		Uint32 fps_lasttime = SDL_GetTicks();
		Uint32 fps_current = 0;
		Uint32 fps_frames = 0;
		
    while (gameRunning) {

			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT) 
					gameRunning = false;
			}

			const bool* currentKeyStates = SDL_GetKeyboardState(NULL);


			float x, y;
			SDL_GetMouseState(&x, &y);

			float newTime = utils::hireTimeInSeconds();
			float deltaTime = newTime - prevTime;
			prevTime = newTime;

			accumulator += std::min(deltaTime, maxTimeStep);

			while (accumulator >= timeStep) {
				world.setWorldUpdate();
				world.handleEvent(currentKeyStates, x, y);
				window.handleEvent(currentKeyStates, x, y);
				world.updateWorld();
				accumulator -= timeStep;
			}

			window.clear();

			window.renderWorld(world);
			fps_frames++;
			if (fps_lasttime < SDL_GetTicks() - 1000) {
				fps_lasttime = SDL_GetTicks();
				fps_current = fps_frames;
				fps_frames = 0;
			}

			//window.renderDebug(fps_current);

			window.display();

			if (currentKeyStates[SDL_SCANCODE_Q]) {
				break;
			}
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}


