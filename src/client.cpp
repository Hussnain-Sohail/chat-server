#include <iostream>
#include <boost/asio.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "messageHandler.hpp"
#include "resource.hpp"
#include "client.hpp"

boost::asio::awaitable<void> Client::Start()
{
    try
    {
        ImGuiIO &io = ImGui::GetIO();
        SDL_Event event;
        bool running{true};

        bool b{true};
        while (running)
        {
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("imgui", &b);
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL3_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                    running = false;
            }

            io.FontGlobalScale = 3;
            ImGui::Text("Works ?");
            io.FontGlobalScale = 2;
            ImGui::Text("hello world");
            ImGui::End();

            SDL_RenderClear(rm.giveRender());

            ImGui::Render();
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rm.giveRender());
            SDL_RenderPresent(rm.giveRender());
        }

        co_return;
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}