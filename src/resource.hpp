#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <utility>

#pragma once
class SDL_Resource_Manager
{
public:
    bool init(int width = 0, int height = 0);
    SDL_Resource_Manager() = default;
    ~SDL_Resource_Manager()
    {
        cleanUp();
    }

    SDL_Resource_Manager(const SDL_Resource_Manager &) = delete;
    SDL_Resource_Manager &operator=(const SDL_Resource_Manager &) = delete;

    SDL_Resource_Manager(SDL_Resource_Manager &&) = delete;
    SDL_Resource_Manager &operator=(SDL_Resource_Manager &&) = delete;

    inline SDL_Window *giveWidnow()
    {
        return window;
    }
    inline SDL_Renderer *giveRender()
    {
        return render;
    }

    const inline std::pair<int, int> &giveWinDimensions()
    {
        return dimensions;
    }

private:
    void cleanUp() noexcept;
    SDL_Window *window = nullptr;
    SDL_Renderer *render = nullptr;
    std::pair<int, int> dimensions;
};