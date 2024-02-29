#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <array>
#include <chrono>
#include <format>

using asio::ip::udp;
using namespace std;
using namespace std::chrono;

class Server
{
public:
    Server(asio::io_context &io_context)
        : _socket{io_context, udp::endpoint(udp::v4(), 3000)}
    {
        receiveFromPeer();
    }

protected:
    void receiveFromPeer()
    {
        // lamda signature
        _socket.async_receive_from(
            asio::buffer(_recvBuffer), _peerEndPoint,
            [this](std::error_code ec, std::size_t bytesRecved)
            {
                if (!ec)
                {
                    sendToPeer();
                }
            });
    }

    void sendToPeer()
    {
        // server local timestamp
        auto current{current_zone()};
        auto nowUTC{system_clock::now()};
        auto nowInCurrentTimezone{current->to_local(nowUTC)};
        auto s{format("{:%d-%m-%Y %H:%M:%OS}", nowInCurrentTimezone)};

        _socket.async_send_to(
            asio::buffer(s), _peerEndPoint,
            [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/)
            {
                receiveFromPeer();
            });
    }

private:
    udp::socket _socket;
    udp::endpoint _peerEndPoint; // p2p
    array<char, 128> _recvBuffer;
};

int main()
{
    try
    {
        asio::io_context io_context;
        Server server{io_context};
        // blocking unless runtime exception
        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}