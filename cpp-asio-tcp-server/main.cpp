#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

using asio::ip::tcp;

int main()
{
    try
    {
        asio::io_context io_context;
        // daytime 
        // tcp  Service names and port numbers: http -> 80
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));
        while (true)
        {
            tcp::socket socket(io_context);
            // kernel blocking call
            // epoll
            acceptor.accept(socket);
            asio::error_code ignored_error;
            asio::write(socket, asio::buffer("Hello from tcp server"), ignored_error);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}