#include <boost/asio.hpp>
#include <openssl/rand.h>

#pragma once
using boost::asio::ip::tcp;
class Socket
{
private:
    std::string socketID;
    tcp::socket socket;

    void assignID()
    {
        unsigned char buffer[10];
        if ((RAND_bytes(buffer, sizeof(buffer))) != 1)
        {
            std::cout << "could not assign id" << std::endl;
            return;
        }

        const char *readable = OPENSSL_buf2hexstr(buffer, sizeof(buffer));
        socketID = readable;
    }

public:
    const std::string &giveSocketID()
    {
        return socketID;
    }

    tcp::socket &giveSocket()
    {
        return socket;
    }

    Socket(boost::asio::io_context &io) : socket(io)
    {
        assignID();
    }

    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;

    Socket(Socket &&) = delete;
    Socket &operator=(Socket &&) = delete;

    ~Socket() = default;
};