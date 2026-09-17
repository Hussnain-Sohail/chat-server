#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "resource.hpp"

bool SDL_Resource_Manager::init(int width, int height)
{
    try
    {
        if (!(SDL_Init(SDL_INIT_VIDEO)))
            throw(SDL_GetError());

        const SDL_DisplayMode *dm = SDL_GetDesktopDisplayMode(1);
        if (!dm)
        {
            std::cout << SDL_GetError();
            return false;
        }

        if (width == 0)
            width = dm->w;
        if (height == 0)
            height = dm->h;

        dimensions = std::pair<int, int>{width, height};
        window = SDL_CreateWindow("window", width, height, SDL_WINDOW_RESIZABLE);
        if (!window)
        {
            std::cout << SDL_GetError();
            return false;
        }

        render = SDL_CreateRenderer(window, nullptr);
        if (!render)
        {
            std::cout << SDL_GetError();
            return false;
        }
        return true;
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
        return false;
    }
}

void SDL_Resource_Manager::cleanUp() noexcept
{
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
}