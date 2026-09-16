#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "socket.hpp"
#include "resource.hpp"
#pragma once
using boost::asio::ip::tcp;
class Client
{
private:
    boost::asio::io_context &io;
    Socket socket{io};
    tcp::resolver resolver{io};
    std::thread t1;
    SDL_Resource_Manager rm;

    boost::asio::awaitable<bool>
    Connect(const std::string &url, const std::string &port);

    bool getMessage();

    boost::asio::awaitable<bool>
    Send(const std::string &message);

    void Helper();

    boost::asio::awaitable<void>
    Read();

public:
    Client(boost::asio::io_context &io) : io(io) {}

    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    Client(Client &&) = delete;
    Client &operator=(Client &&) = delete;

    boost::asio::awaitable<void>
    Start(const std::string &url, const std::string &port);

    ~Client()
    {
        t1.join();
    }
};