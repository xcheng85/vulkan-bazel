#include <iostream>
#include <string>
#include <memory>
#include <asio.hpp>
#include <array>

using asio::ip::tcp;

// server creates a new socket for each connection

namespace v2
{
    // enable_shared_from_this, allow multiple share pointer co-owns the same native pointer
    // Session lifecyle started after tcp accepted by server
    // Session lifecyle ends ?
    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(tcp::socket socket) : _socket{std::move(socket)}
        {
            std::cout << "Session: ctor\n";
            // cannot start here due to shared_from_this() must called after object is constructed
            // startAsyncCallChain();
        }
        
        ~Session() {
            std::cout << "Session: dtor\n";
        }

        void startAsyncCallChain()
        {
            readFromPeer();
        }

    protected:
        void readFromPeer()
        {
            // A trick for keeping an object alive in a C++ lambda
            // while still being able to use the this keyword to refer to it
            auto self(shared_from_this());
            // _recvBuffer size is allocated on stack
            _socket.async_read_some(asio::buffer(_recvBuffer),
                                    [this, self](std::error_code ec, std::size_t length)
                                    {
                                        if (!ec)
                                        {
                                            writeToPeer(length);
                                        }
                                    });
        }

        void writeToPeer(std::size_t length)
        {
            auto self(shared_from_this());
            // partial buffer
            asio::async_write(_socket, asio::buffer(_recvBuffer.data(), length),
                              [this, self](std::error_code ec, std::size_t /*length*/)
                              {
                                  if (!ec)
                                  {
                                      readFromPeer();
                                  }
                              });
        }

        // https://devblogs.microsoft.com/oldnewthing/20190104-00/?p=100635
    private:
        tcp::socket _socket; // stream-oriented socket
        std::array<char, 128> _recvBuffer;
    };

    class Server
    {
    public:
        Server(asio::io_context &ctx)
            : _acceptor{ctx, tcp::endpoint{tcp::v4(), 3000}}
        {
            start_accept();
        }

    protected:
        void start_accept()
        {
            std::cout << "accept\n";
            _acceptor.async_accept([this](std::error_code ec, tcp::socket socket)
                                   {
                                   if (!ec)
                                   {
                                    std::cout << socket.remote_endpoint().address().to_string() << std::endl;
                                       // start new session for this connection
                                       std::make_shared<Session>(std::move(socket))->startAsyncCallChain();
                                   }
                                   // start_accept() to initiate the next accept operation.
                                   this->start_accept(); });
        }

    private:
        tcp::acceptor _acceptor;
    };
}