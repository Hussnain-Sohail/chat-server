#include <boost/asio.hpp>
#include <coroutine>
#include <array>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include "messageHandler.hpp"
#include "resource.hpp"

boost::asio::awaitable<bool> messageHandler::Connect(const std::string &host, const std::string &port)
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
// not passing message by referance since client uses one message object which can be changed during a pending operation
boost::asio::awaitable<bool> messageHandler::Send(const std::string message)
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

boost::asio::awaitable<void> messageHandler::Read()
{
    try
    {
        std::string readBuffer;
        boost::system::error_code ec;

        int bytesRead = co_await boost::asio::async_read(socket.giveSocket(), boost::asio::buffer(readBuffer), boost::asio::transfer_at_least(1), boost::asio::redirect_error(boost::asio::use_awaitable, ec));

        if (ec)
        {
            std::cout << ec.message() << std::endl;
            co_return;
        }

        ImGui::Text(readBuffer.c_str());
        co_await Read();
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}
