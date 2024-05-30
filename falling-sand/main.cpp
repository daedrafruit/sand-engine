
#include <iostream>
#include <SDL.h>
#include <vector>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"

int main(int argc, char* args[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0) {
        std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
    }

    int cellSize = 20;
    int windowWidth = 1280;
    int windowHeight = 720;

    RenderWindow window("Falling Sand", windowWidth, windowHeight);
    int windowRefreshRate = window.getRefreshRate();

    std::vector <Entity> entities = {Entity(100, 0, 250, 200, 70)
                                     };

    bool gameRunning = true;
    SDL_Event event;
    
    const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    while (gameRunning) 
    {

        int startTicks = SDL_GetTicks();

        float newTime = utils::hireTimeInSeconds();
        float frameTime = newTime - currentTime;

        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= timeStep)
        {

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    gameRunning = false;
            }
            accumulator -= timeStep;
        }

        const float alpha = accumulator / timeStep;
        

        window.clear();

        for (Entity& e : entities) 
        {
            
            e.setY(e.getY() + cellSize);
            window.render(e, cellSize);
        }

        window.display();

        int frameTicks = SDL_GetTicks() - startTicks;

        if (frameTicks < 1000 / window.getRefreshRate())
            SDL_Delay(1000 / window.getRefreshRate() - frameTicks);

    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
