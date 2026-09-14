#include <iostream>
#include <boost/asio.hpp>
#include <boost/url.hpp>

#pragma once
using boost::asio::ip::tcp;
class Client
{
private:
    tcp::socket socket;
    tcp::resolver resolver;
    boost::urls::url url;

    boost::asio::awaitable<bool>
    Connect(const std::string &url, const std::string &port);

    boost::asio::awaitable<void>
    Read();

public:
    Client() = default;

    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    Client(Client &&) = delete;
    Client &operator=(Client &&) = delete;

    ~Client() = default;
};