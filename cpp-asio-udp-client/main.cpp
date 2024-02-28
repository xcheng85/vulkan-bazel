#include <iostream>
#include <asio.hpp>
#include <array>
#include <string>

using asio::ip::udp;

int main()
{
    try
    {
        asio::io_context io_context;

        udp::resolver resolver(io_context);
        // list of endpoints
        udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "localhost", "3000");
        // resolve: return at least one endpoint
        udp::endpoint firstEndpoint = *(endpoints.begin());
        // udp socket
        // datagram-oriented socket
        // open by udp
        udp::socket socket(io_context);
        socket.open(udp::v4());

        // buffer size need contract between client-server
        std::string sendMsg{"hello from client"};
        // std::vector<char> d2(128);
        // std::array<char, 128> buf;
        // char data_[128];

        // client --> server
        // blocking call
        socket.send_to(asio::buffer(sendMsg.data(), sendMsg.size()), firstEndpoint);

        std::array<char, 32> buf;
        udp::endpoint serverEp;
        // blocking call
        size_t byteSizeRecv = socket.receive_from(asio::buffer(buf), serverEp);
        std::cout << std::string{buf.data()} << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}