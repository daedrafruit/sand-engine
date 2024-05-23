
#include <iostream>
#include <SDL.h>

#include "RenderWindow.hpp"

int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
    }

    RenderWindow window("Falling Sand", 1280, 720);

    bool gameRunning = true;

    SDL_Event event;

    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
