#include <iostream>
#include <asio.hpp>
#include <array>

using asio::ip::tcp;

int main()
{
    try
    {
        asio::io_context io_context;

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints =
            resolver.resolve("localhost", "3000");

        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        for (;;)
        {
            std::array<char, 128> buf;
            asio::error_code error;
            // system blocking call
            // asio::buffer: mutable buffer
            size_t len = socket.read_some(asio::buffer(buf), error);
            if (error == asio::error::eof) {
                break; // Connection closed cleanly by server.
            }
            else if (error) {
                throw asio::system_error(error);
            }
            // write binary data char*
            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}