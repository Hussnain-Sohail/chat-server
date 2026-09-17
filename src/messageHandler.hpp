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
    char inputBuffer[100] = {};
    std::string message;
    std::vector<std::string> messages;
    std::mutex mtx;

public:
    boost::asio::awaitable<bool>
    Connect(const std::string &url, const std::string &port);

    boost::asio::awaitable<bool>
    Send(const std::string &message);

    boost::asio::awaitable<void>
    Read();

    inline void printMessages()
    {
        for (const std::string &x : messages)
            ImGui::Text(x.c_str());
    }

public:
    messageHandler(boost::asio::io_context &io) : io(io) {}

    messageHandler(const messageHandler &) = delete;
    messageHandler &operator=(const messageHandler &) = delete;

    messageHandler(messageHandler &&) = delete;
    messageHandler &operator=(messageHandler &&) = delete;

    boost::asio::awaitable<void>
    Start(const std::string &url, const std::string &port);
};