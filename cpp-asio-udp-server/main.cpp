#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>
#include <array>

using asio::ip::udp;
using namespace std;

int main()
{
    try
    {
        asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 3000));
        while (true)
        {
            udp::endpoint remote_endpoint;
            // receive a datagram with the endpoint of the sender.
            // blocking call
            array<char, 32> buf;
            auto bytesRecv = socket.receive_from(asio::buffer(buf), remote_endpoint);
            cout << remote_endpoint.address().to_string() << endl;
             cout << std::string{std::move(buf.data())} << endl;
            // p2p style
            // Send a datagram to the specified endpoint.
            asio::error_code ignored_error;
            socket.send_to(asio::buffer("hello from server"), remote_endpoint, 0, ignored_error);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}