
#include <iostream>
#include <SDL.h>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "SandWorld.hpp"

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    const int cellSize = 5;
    const int windowWidth = 1280;
    const int windowHeight = 720;

    RenderWindow window("Falling Sand", windowWidth, windowHeight);

    SandWorld world(windowHeight, windowWidth, cellSize);

    bool gameRunning = true;
    SDL_Event event;
    
    const float timeStep = 0.02f;
    const float maxTimeStep = 0.25f;
    float accumulator = 0.0f;
    float prevTime = utils::hireTimeInSeconds();

    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                gameRunning = false;
        }

        world.mouseEvent(window);

        float newTime = utils::hireTimeInSeconds();
        float deltaTime = newTime - prevTime;
        prevTime = newTime;

        accumulator += std::min(deltaTime, maxTimeStep);

        while (accumulator >= timeStep) {
					world.updateWorld();
					accumulator -= timeStep;
        }

        window.clear();

        world.renderWorld(window);

        window.display();
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}


