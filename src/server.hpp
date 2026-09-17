#include <boost/asio.hpp>
#include <unordered_map>

using boost::asio::ip::tcp;
class Server
{
private:
    size_t totalConnections{0};
    std::unordered_map<std::string, size_t> connectedSockets;

    boost::asio::io_context &io;
    tcp::acceptor acceptor{io};

    boost::asio::awaitable<void>
    AcceptConnection();

    boost::asio::awaitable<void>
    listenForMesssages();

    boost::asio::awaitable<bool>
    broadcastMessage();

    void removeConnection(const std::string &connection) noexcept;

    boost::asio::awaitable<void>
    Start();

public:
    Server(boost::asio::io_context &io) : io(io)
    {
        boost::asio::co_spawn(io, Start(), boost::asio::detached);
    }

    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    Server(Server &&) = delete;
    Server &operator=(Server &&) = delete;

    ~Server() = default;
};