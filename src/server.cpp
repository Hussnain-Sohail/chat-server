#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include "server.hpp"
#include "socket.hpp"

boost::asio::awaitable<void> Server::AcceptConnection()
{
    try
    {
        Socket socket{io};
        co_await acceptor.async_accept(socket.giveSocket(), boost::asio::use_awaitable);

        co_await AcceptConnection();
    }
    catch (std::exception &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}