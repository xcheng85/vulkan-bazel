#include <iostream>
#include <string>
#include <memory>
#include <asio.hpp>
#include "./v1/server.h"
#include "./v2/server.h"

using asio::ip::tcp;

int main()
{
    try
    {
        asio::io_context io_context;
        v2::Server server(io_context);
        // node.js style event loop
        // async_accept never end
        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;

    return 0;
}