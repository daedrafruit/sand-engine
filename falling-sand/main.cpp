
#include <iostream>
#include <SDL.h>
#include <vector>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"

int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    int cellSize = 20;
    int windowWidth = 1280;
    int windowHeight = 720;

    RenderWindow window("Falling Sand", windowWidth, windowHeight);

    std::vector<Entity> entities = {
        Entity(100, 0, 250, 200, 70)
    };

    bool gameRunning = true;
    SDL_Event event;
    
    const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    while (gameRunning) 
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                gameRunning = false;
        }

        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;
        currentTime = newTime;

        if (frameTime > 0.25f)
            frameTime = 0.25f;

        accumulator += frameTime;

        while (accumulator >= timeStep)
        {
            for (Entity& e : entities) 
            {

                e.setY(e.getY() + cellSize);            
            }
            accumulator -= timeStep;
        }

        window.clear();

        for (Entity& e : entities) 
        {
            window.render(e, cellSize);
        }

        window.display();
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
