
#include <iostream>
#include <SDL.h>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"

int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
    }

    std::vector <Entity> entities = {Entity(100, 100, 250, 200, 70),
                                     Entity(100, 200, 0, 0, 255)};

    RenderWindow window("Falling Sand", 1280, 720);

    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }
        
        window.clear();

        for (Entity& e : entities) {
            window.render(e);
        }

        window.display();

    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
