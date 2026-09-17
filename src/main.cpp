#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "client.hpp"

// g++ main.cpp resource.cpp  messageHandler.cpp client.cpp  -lcrypto -fcoroutines  ../include/imgui.cpp ../include/imgui_demo.cpp ../include/imgui_draw.cpp ../include/imgui_tables.cpp ../include/imgui_widgets.cpp ../include/backends/imgui_impl_sdl3.cpp ../include/backends/imgui_impl_sdlrenderer3.cpp  -I ../include -I../include/backends -lSDL3 -std=c++23 -o main

int main()
{
    boost::asio::io_context io;
    Client client{io};
    boost::asio::co_spawn(io, client.Start(), boost::asio::detached);
    io.run();
}