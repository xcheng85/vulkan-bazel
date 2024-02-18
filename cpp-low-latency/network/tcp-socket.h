#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "socket.h"

namespace cpp_low_latency
{
    namespace network
    {
        using namespace std;
        class TcpSocket
        {
        public:
            explicit TcpSocket(unique_ptr<Socket> socket,
                               function<void(TcpSocket *s, timeval kernelTime)> recvCb)
                : _socket{move(socket)},
                  _sendBuffer(TcpBufferSize),
                  _receiveBuffer(TcpBufferSize),
                  _recvCb{recvCb}
            {
            }
            TcpSocket() = delete;
            // TcpSocket(const TcpSocket &) = delete;
            // TcpSocket(TcpSocket &&) = delete;
            // TcpSocket &operator=(const TcpSocket &) = delete;
            // TcpSocket &operator=(TcpSocket &&) = delete;

            ~TcpSocket()
            {
            }

            const auto &socket() const
            {
                return *_socket;
            }

            void sendToBuffer(const void *data, size_t length) noexcept
            {
                memcpy(_sendBuffer.data() + _sendBufferNextOffset, data, length);
                _sendBufferNextOffset += length;
            }

            bool sendAndRecv()
            {
                // system call
                // receive multiple messages from a socket using a single system call
                // Non-blocking call to read available data.

                // msg_name: it points to a generic socket address;
                // AF_UNIX: sockaddr_un
                // AF_INET (ipv4): sockaddr_in
                // AF_INET6: sockaddr_in6

                // msg_iov: vector of data send/receive into
                // pointer to the data: _receiveBuffer.data() + _receiveBufferNextOffset
                // length of data: TcpBufferSize - _receiveBufferNextOffset (leftover)

                // msg_control:
                // network interface of recv packet.
                // From the msg_control, you can find interesting things like the destination IP of the packet
                // (useful for multicast) and the contents of the TOS/DSCP byte in the IP header
                // (useful for custom congestion control protocols), among others.
                // In most cases, you'll need to make a setsockopt call to enable receiving this data.
                // In the examples given, the IP_PKTINFO and IP_TOS options need to be enabled.

                // msg_control is a struct cmsghdr
                // received ancillary buffer

                // This code looks for the SCM_TIMESTAMP option in a received ancillary buffer:
                // that is why it is timeval
                char ctrl[CMSG_SPACE(sizeof(timeval))];
                auto cmsg = reinterpret_cast<struct cmsghdr *>(&ctrl);

                iovec iov{_receiveBuffer.data() + _receiveBufferNextOffset, TcpBufferSize - _receiveBufferNextOffset};
                msghdr msg{&_socketArress, sizeof(_socketArress), &iov, 1, ctrl, sizeof(ctrl), 0};
                const auto size = recvmsg(_socket->fd(), &msg, MSG_DONTWAIT);
                if (size > 0)
                {
                    _receiveBufferNextOffset += size;
                    timeval kernelTimeVal;
                    if (cmsg->cmsg_level == SOL_SOCKET &&
                        cmsg->cmsg_type == SCM_TIMESTAMP &&
                        cmsg->cmsg_len == CMSG_LEN(sizeof(kernelTimeVal)))
                    {
                        memcpy(&kernelTimeVal, CMSG_DATA(cmsg), sizeof(kernelTimeVal));
                    }
                    _recvCb(this, kernelTimeVal);
                }

                if (_sendBufferNextOffset > 0)
                {
                    // Non-blocking call to send data.
                    const auto numberBytesSent = send(_socket->fd(),
                                                      _sendBuffer.data(),
                                                      _sendBufferNextOffset,
                                                      MSG_DONTWAIT | MSG_NOSIGNAL);
                }
                // the whole sendBuffer is flushed out
                _sendBufferNextOffset = 0;
                return size > 0;
            }


            string recv() const {
                return {_receiveBuffer.data(), _receiveBufferNextOffset};
            }
        private:
            unique_ptr<Socket> _socket;
            static constexpr size_t TcpBufferSize = 16 * 1024 * 1024;

            vector<char> _sendBuffer;
            size_t _sendBufferNextOffset{0}; // byte

            vector<char> _receiveBuffer;
            size_t _receiveBufferNextOffset{0}; // byte

            struct sockaddr_in _socketArress
            {
            };

            function<void(TcpSocket *s, timeval kernelTime)> _recvCb;
        };
    }
}