#include <boost/asio.hpp>
#include <coroutine>
#include <array>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "client.hpp"
#include "resource.hpp"

boost::asio::awaitable<bool> Client::Connect(const std::string &host, const std::string &port)
{
    try
    {
        if (host.empty() || port.empty())
            co_return false;

        boost::system::error_code ec;

        auto endpoints = co_await resolver.async_resolve(host, port, boost::asio::redirect_error(boost::asio::use_awaitable, ec));
        if (ec)
        {
            std::cout << ec.message() << std::endl;
            co_return false;
        }

        co_await boost::asio::async_connect(socket.giveSocket(), endpoints, boost::asio::redirect_error(boost::asio::use_awaitable, ec));
        if (ec)
        {
            std::cout << ec.message() << std::endl;
            co_return false;
        }

        std::cout << "Connected" << std::endl;
        co_return true;
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}

bool Client::getMessage()
{
    std::cout << "Enter message" << std::endl;
    std::string message;
    std::getline(std::cin, message);
    if (message == ".q")
        return true;

    std::string request =
        "POST /post HTTP/1.1\r\n"
        "Host: httpbin.org\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " +
        std::to_string(message.size()) + "\r\n"
                                         "Connection: close\r\n"
                                         "\r\n" +
        message;

    boost::asio::post(io, [&, this]()
                      { boost::asio::co_spawn(io, Send(request), boost::asio::detached); });
    return false;
}

boost::asio::awaitable<bool> Client::Send(const std::string &message)
{
    try
    {
        if (!socket.giveSocket().is_open())
        {
            std::cout << "Message not sent. Connected to no server" << std::endl;
            co_return false;
        }

        boost::system::error_code ec;
        co_await boost::asio::async_write(socket.giveSocket(), boost::asio::buffer(message), boost::asio::redirect_error(boost::asio::use_awaitable, ec));
        if (ec)
        {
            std::cout << "Message not sent" << std::endl;
            std::cout << ec.message() << std::endl;
            co_return false;
        }

        std::cout << "sent" << std::endl;
        co_return true;
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}

void Client::Helper()
{
    try
    {
        bool created = rm.init();
        if (!created)
        {
            std::cout << "coult not initialize sdl" << std::endl;
            return;
        }

        IMGUI_CHECKVERSION();
        ImGui_ImplSDL3_InitForSDLRenderer(rm.giveWidnow(), rm.giveRender());
        ImGui_ImplSDLRenderer3_Init(rm.giveRender());

        bool running{true};
        SDL_Event event;
        while (running)
        {
            SDL_RenderClear(rm.giveRender());
            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL3_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                    running = false;
            }

            ImGui::Render();
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), rm.giveRender());
            SDL_RenderPresent(rm.giveRender());
        }
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
};

boost::asio::awaitable<void> Client::Read()
{
    try
    {
        std::array<char, 1024> readBuffer;
        boost::system::error_code ec;

        int bytesRead = co_await boost::asio::async_read(socket.giveSocket(), boost::asio::buffer(readBuffer), boost::asio::transfer_at_least(1), boost::asio::redirect_error(boost::asio::use_awaitable, ec));

        if (ec)
        {
            std::cout << ec.message() << std::endl;
            co_return;
        }

        std::cout << "Message ";
        std::cout.write(readBuffer.data(), bytesRead);

        co_await Read();
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}

boost::asio::awaitable<void> Client::Start(const std::string &url, const std::string &port)
{
    try
    {
        bool connected = co_await Connect(url, port);
        if (!connected)
        {
            std::cout << "Could not connect to server" << std::endl;
            co_return;
        }

        boost::asio::co_spawn(io, Read(), boost::asio::detached);

        t1 = std::thread{Helper};
        co_return;
    }
    catch (std::system_error &error)
    {
        std::cout << "message " << error.what() << std::endl;
        std::cout << "error code " << error.code().value() << std::endl;
    }
    catch (std::exception &exc)
    {
        std::cout << "start catch" << std::endl;
        std::cout << exc.what() << std::endl;
    }
}