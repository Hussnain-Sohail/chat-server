#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <mutex>
#include <imgui.h>
#include "socket.hpp"
#include "resource.hpp"
#pragma once
using boost::asio::ip::tcp;
class messageHandler
{
private:
    boost::asio::io_context &io;
    Socket socket{io};
    tcp::resolver resolver{io};

public:
    boost::asio::awaitable<bool>
    Connect(const std::string &url, const std::string &port);

    boost::asio::awaitable<bool>
    Send(const std::string message);

    boost::asio::awaitable<void>
    Read();

public:
    messageHandler(boost::asio::io_context &io) : io(io) {}

    messageHandler(const messageHandler &) = delete;
    messageHandler &operator=(const messageHandler &) = delete;

    messageHandler(messageHandler &&) = delete;
    messageHandler &operator=(messageHandler &&) = delete;
};