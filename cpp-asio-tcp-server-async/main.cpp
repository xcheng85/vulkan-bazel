#include <iostream>
#include <string>
#include <memory>
#include <asio.hpp>

using asio::ip::tcp;

class Connection
{
public:
    static std::shared_ptr<Connection> create(asio::io_context &io_context)
    {
        // cannot use make_shared here due to private ctor
        std::shared_ptr<Connection> p(new Connection(io_context));
        return p;
    }

    tcp::socket &socket()
    {
        return _socket;
    }

    void start()
    {
        // use lamda instead of bind
        _message = "new connection started";
        asio::async_write(_socket, asio::buffer(_message),
                          [this](std::error_code error, std::size_t /*n*/)
                          {
                              if (!error)
                              {
                                  std::cout << "Done with async write\n";
                              }
                              else
                              {
                                  std::cerr << "Error async_write: " << error.message() << "\n";
                              }
                          }

        );
    }

private:
    // factory pattern
    explicit Connection(asio::io_context &ctx)
        : _socket{ctx}
    {
    }

    tcp::socket _socket; // stream-oriented socket
    std::string _message;
};

class Server
{
public:
    Server(asio::io_context &ctx)
        : _ctx{ctx},
          _acceptor{ctx, tcp::endpoint{tcp::v4(), 3000}}
    {
        start_accept();
    }

protected:
    void start_accept()
    {
        std::cout << "accept\n";
        auto c = Connection::create(_ctx);
        // use lamda
        // This function is used to asynchronously accept a new connection into a socket
        // &: include reference to c and this.
        _acceptor.async_accept(c->socket(), [&](const asio::error_code &error)
                               {
                                   if (!error)
                                   {
                                        // server write to socket
                                       c->start();
                                   }
                                   // start_accept() to initiate the next accept operation.
                                   this->start_accept(); });
    }

private:
    asio::io_context &_ctx;
    tcp::acceptor _acceptor;
};

int main()
{
    try
    {
        asio::io_context io_context;
        Server server(io_context);
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