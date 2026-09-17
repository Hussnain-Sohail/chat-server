#include <iostream>
#include <boost/asio.hpp>
#include "messageHandler.hpp"
#include "resource.hpp"

#pragma once
class Client
{
private:
    boost::asio::io_context &io;
    Socket socket{io};
    SDL_Resource_Manager rm;
    std::string message;

public:
    boost::asio::awaitable<void>
    Start();

    Client(boost::asio::io_context &io) : io(io)
    {
        bool initSuccess = rm.init();
        if (!initSuccess)
            std::cout << "Could not initialize resource manager" << std::endl;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForSDLRenderer(rm.giveWidnow(), rm.giveRender());
        ImGui_ImplSDLRenderer3_Init(rm.giveRender());
    }

    ~Client()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
};