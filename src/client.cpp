#include <boost/asio.hpp>
#include <boost/url.hpp>
#include <coroutine>
#include <array>
#include "client.hpp"

boost::asio::awaitable<bool> Client::Connect(const std::string &urlToConnect, const std::string &port)
{
    try
    {
        if (urlToConnect.empty())
            co_return false;

        boost::urls::url url{urlToConnect};
        boost::system::error_code ec;

        auto endpoints = co_await resolver.async_resolve(url.host(), port, boost::asio::redirect_error(boost::asio::use_awaitable, ec));
        if (ec)
        {
            std::cout << ec.message() << std::endl;
            co_return false;
        }

        co_await boost::asio::async_connect(socket, endpoints, boost::asio::redirect_error(boost::asio::use_awaitable, ec));
        if (ec)
        {
            std::cout << ec.message() << std::endl;
            co_return false;
        }

        std::cout << "Connected" << std::endl;
        co_return false;
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
    boost::asio::post(io, [&, this]()
                      { boost::asio::co_spawn(io, Send(message), boost::asio::detached); });
    return false;
}

boost::asio::awaitable<bool> Client::Send(const std::string &message)
{
    try
    {
        if (!socket.is_open())
        {
            std::cout << "Message not sent. Connected to no server" << std::endl;
            co_return false;
        }

        boost::system::error_code ec;
        co_await boost::asio::async_write(socket, boost::asio::buffer(message), boost::asio::redirect_error(boost::asio::use_awaitable, ec));
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

boost::asio::awaitable<void> Client::Read()
{
    try
    {
        std::array<char, 1024> readBuffer;
        boost::system::error_code ec;

        int bytesRead = co_await boost::asio::async_read(socket, boost::asio::buffer(readBuffer), boost::asio::transfer_at_least(1), boost::asio::redirect_error(boost::asio::use_awaitable, ec));

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

        const auto helper = [&]()
        {
            while (true)
            {
                bool userExited = getMessage();
                if (userExited)
                    break;
            }
        };

        t1 = std::thread{helper};
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}