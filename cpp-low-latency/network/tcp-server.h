#pragma once
#include <algorithm>
#include <sys/epoll.h>
#include "tcp-socket.h"

// epoll: Linux kernel system call for a scalable I/O event notification mechanism

// new connections
// dead connections
// existing connections for reading data

namespace cpp_low_latency
{
    namespace network
    {
        using namespace std;
        class TcpServer
        {
        public:
            explicit TcpServer(unique_ptr<TcpSocket> socket,
                               function<void(TcpSocket *s, timeval kernelTime)> recvCb,
                               std::function<void()> allRecvedCb)
                : _socket{move(socket)},
                  _epfd{epoll_create(1)},
                  _recvCb{recvCb},
                  _allRecvedCb{allRecvedCb}
            {
                ASSERT(_epfd >= 0, "epoll_create() failed error:" + std::string(std::strerror(errno)));
                ASSERT(monitor(*_socket, true), "epoll_ctl() failed. error:" + std::string(std::strerror(errno)));
            }

            ~TcpServer()
            {
                close(_epfd);
            }

            void sendAndRecv() noexcept
            {
                auto recv = false;
                std::for_each(_recvSockets.begin(), _recvSockets.end(), [&recv](auto socket)
                              { recv |= socket->sendAndRecv(); });

                if (recv)
                {
                    _allRecvedCb();
                }

                std::for_each(_sendSockets.begin(), _sendSockets.end(), [](auto socket)
                              { socket->sendAndRecv(); });
            }

            void poll()
            {
                // clean up disconnected sockets
                cleanDisconnectedSocket();

                bool newConnection = false;
                // events: information from the ready list about file descriptors in the interest list
                // that have some events available
                const int max_events = 1 + _sendSockets.size() + _recvSockets.size();
                // block call with timeout 0
                // returns the number of file descriptors ready for the requested I/O
                const int n = epoll_wait(_epfd, _events, max_events, 0);
                for (int i = 0; i < n; ++i)
                {
                    const auto &event = _events[i];
                    // The events field is a bit mask that indicates the events that
                    // have occurred for the corresponding open file description
                    auto tcpSocket = reinterpret_cast<TcpSocket *>(event.data.ptr); // line 47
                    // this socket is ready for read
                    if (event.events & EPOLLIN)
                    {
                        if (tcpSocket == _socket.get())
                        {
                            newConnection = true;
                            continue;
                        }
                        if (std::find(_recvSockets.begin(), _recvSockets.end(), tcpSocket) == _recvSockets.end())
                        {
                            _recvSockets.emplace_back(tcpSocket);
                        }
                    }
                    // this socket is ready for write
                    if (event.events & EPOLLOUT)
                    {
                        if (std::find(_sendSockets.begin(), _sendSockets.end(), tcpSocket) == _sendSockets.end())
                        {
                            _sendSockets.emplace_back(tcpSocket);
                        }
                    }
                    // this socket is disconnected
                    if (event.events & (EPOLLERR | EPOLLHUP))
                    {
                        if (std::find(_disconnectedSockets.begin(), _disconnectedSockets.end(), tcpSocket) == _disconnectedSockets.end())
                        {
                            _disconnectedSockets.emplace_back(tcpSocket);
                        }
                    }
                }
                // may have many new connections in this poll system call
                while (newConnection)
                {
                    sockaddr_storage addr;
                    socklen_t addr_len = sizeof(addr);

                    // accept: system call, only for tcp
                    // extracts the first connection request on the queue of pending connections for the
                    // listening socket
                    // creates a new connected socket returns a new file descriptor referring to that socket.(to the client)
                    // accept will block if the socket is blocking socket !
                    int fd = accept(_socket->socket().fd(), reinterpret_cast<sockaddr *>(&addr), &addr_len);
                    if (fd == -1)
                    {
                        // all the pending connects are accepted
                        break;
                    }

                    auto tcpSocket = make_unique<TcpSocket>(make_unique<Socket>(fd), _recvCb);
                    auto tcpSocketRawPtr = tcpSocket.get();
                    // socket->recv_callback_ = recv_callback_;
                    ASSERT(monitor(*tcpSocket, true), "Unable to monitor tcpSocket. error:" + std::string(std::strerror(errno)));
                    // ownership
                    if (std::find(_sockets.begin(), _sockets.end(), tcpSocket) == _sockets.end())
                    {
                        _sockets.emplace_back(move(tcpSocket));
                    }
                    if (std::find(_recvSockets.begin(), _recvSockets.end(), tcpSocket.get()) == _recvSockets.end())
                    {
                        _recvSockets.emplace_back(tcpSocketRawPtr);
                    }
                }
            }

        protected:
            bool monitor(const TcpSocket &tcpSocket, bool enable)
            {
                //  edge-triggered and level-triggered notification
                // An application that employs the EPOLLET flag should use nonblocking file descriptors
                // EPOLLET: edge-triggered epoll, notified only once that data is ready to read
                // EPOLLIN: availabe for read
                if (enable)
                {
                    epoll_event ev{EPOLLET | EPOLLIN, {reinterpret_cast<void *>(const_cast<TcpSocket *>(&tcpSocket))}};
                    return epoll_ctl(_epfd, EPOLL_CTL_ADD, tcpSocket.socket().fd(), &ev) == 0;
                }
                else
                {
                    return epoll_ctl(_epfd, EPOLL_CTL_DEL, tcpSocket.socket().fd(), nullptr) == 0;
                }
            }

            void cleanDisconnectedSocket()
            {
                for (const auto &socket : _disconnectedSockets)
                {
                    // delist from monitor
                    ASSERT(monitor(*socket, false), "epoll_ctl() failed. error:" + std::string(std::strerror(errno)));
                    // vector erase pattern
                    // remove is to swap with end iter
                    _sendSockets.erase(remove(_sendSockets.begin(), _sendSockets.end(), socket), _sendSockets.end());
                    _recvSockets.erase(remove(_sendSockets.begin(), _sendSockets.end(), socket), _sendSockets.end());

                    auto it = find_if(_sockets.begin(), _sockets.end(), [&](unique_ptr<TcpSocket> &psocket)
                                      { return psocket.get() == socket; });
                    _sockets.erase(it);
                }
                // swap idiom for reset vector
                vector<TcpSocket *> empty;
                _disconnectedSockets.swap(empty);
            }

        private:
            unique_ptr<TcpSocket> _socket;
            // epoll instance referred to by the file descriptor
            // epoll_create: create one epoll instance
            int _epfd;
            // monitoring list that return events related to a socket
            static constexpr size_t MaxMonitoringSocketSize = 16;
            epoll_event _events[MaxMonitoringSocketSize];
            vector<unique_ptr<TcpSocket>> _sockets;
            vector<TcpSocket *> _sendSockets, _recvSockets, _disconnectedSockets;

            function<void(TcpSocket *s, timeval kernelTime)> _recvCb;
            std::function<void()> _allRecvedCb;
        };
    }
}