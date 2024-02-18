#pragma once

#include <vector>
#include <string>
#include <format>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
// POSIX standard
#include <netdb.h>
// for: IPPROTO_TCP
#include <netinet/in.h>
// for: TCP_NODELAY
#include <netinet/tcp.h>
#include <arpa/inet.h>
// Standard C library (libc, -lc)
#include <ifaddrs.h>
#include <sys/socket.h>
// system call (kernel)
#include <fcntl.h>

#include "../common.h"

// Socket class used by both server and client (tcp / udp)
// D principle
namespace cpp_low_latency
{
    namespace network
    {
        using namespace std;
        using namespace cpp_low_latency::common;
        struct SocketConfig
        {
            std::string ip;
            std::string iface;
            int port{-1};
            bool isUdp = false;
            bool isListening = false;
            bool useSoftwareTimestamp = false; // network packets timestamping
        };
        class Socket
        {
        public:
            explicit Socket(const SocketConfig &config) : _fd{createSocket(config)}
            {
            }

            // for accept case
            explicit Socket(int fd) : _fd{fd}
            {
                ASSERT(setSocketNonBlocking(fd) && disableNagleAlgo(fd),
                       "Failed to set non-blocking or no-delay on socket:" + std::to_string(fd));
            }
            Socket() = delete;
            // Socket(const Socket &) = delete;
            // Socket(Socket &&) = delete;
            // Socket &operator=(const Socket &) = delete;
            // Socket &operator=(Socket &&) = delete;

            ~Socket()
            {
                close(_fd);
            }

            inline auto &fd() const
            {
                return this->_fd;
            }

        protected:
            auto getIfaceIP(const std::string &iface)
            {
                char buf[NI_MAXHOST] = {'\0'};
                ifaddrs *ifaddr = nullptr;
                // get linked list of network interface
                if (getifaddrs(&ifaddr) != -1)
                {
                    for (auto *ifa = ifaddr; ifa; ifa = ifa->ifa_next)
                    {
                        // AF_INET: Internet Protocol v4 addresses
                        // AF_INET6: ipv6
                        // ifa_name: Name of this network interface
                        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET && iface == ifa->ifa_name)
                        {
                            // converts a socket address to a corresponding host and service,
                            getnameinfo(ifa->ifa_addr, sizeof(sockaddr_in), buf, sizeof(buf), NULL, 0, NI_NUMERICHOST);
                            break;
                        }
                    }
                    // c-style programming: delete the resource from "getifaddrs"
                    freeifaddrs(ifaddr);
                }

                return std::string{buf};
            }

            // a system call
            // user-level process to request services of the operating system (kernel)
            // triggers a switch to kernel mode
            bool setSocketNonBlocking(int fd)
            {
                // gets the status flags of socket descriptor socket
                // socket, command, args
                const auto flags = fcntl(fd, F_GETFL, 0);
                // already set non block
                if (flags & O_NONBLOCK)
                    return true;
                // set command, bit or
                return (fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1);
            }

            // manipulating options for the socket
            // IPPROTO_TCP: option is interpreted by the TCP
            // only applies to TCP
            bool disableNagleAlgo(int fd)
            {
                int opt = 1;
                auto res = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<void *>(&opt), sizeof(opt));
                return res != -1;
            }

            auto setSOTimestamp(int fd)
            {
                int opt = 1;
                // at socket level
                return (setsockopt(fd, SOL_SOCKET, SO_TIMESTAMP, reinterpret_cast<void *>(&opt), sizeof(opt)) == 0);
            }

            int createSocket(const SocketConfig &config)
            {
                std::string time_str;

                const auto ip = config.ip.empty() ? getIfaceIP(config.iface) : config.ip;

                // specifies criteria for selecting the socket address
                // AI_PASSIVE: The wildcard address is used by applications
                // (typically servers) that intend to accept connections on any of he host's network addresses.
                // AI_NUMERICHOST: numerical network address. 127.0.0.1
                // AI_NUMERICSERV: service must point to a string containing a numeric port number: 8080
                // Protocol family: AF_INET. ipv4
                // UDP: datagram socket, connection less
                // TCP: stream socket: reliable connection-oriented
                // protocal: udp or tcp
                const int input_flags = (config.isListening ? AI_PASSIVE : 0) | (AI_NUMERICHOST | AI_NUMERICSERV);
                const addrinfo hints{input_flags, AF_INET, config.isUdp ? SOCK_DGRAM : SOCK_STREAM,
                                     config.isUdp ? IPPROTO_UDP : IPPROTO_TCP, 0, 0, nullptr, nullptr};
                // header of a linked list
                addrinfo *res = nullptr;
                // getaddrinfo: Internet address
                // addrinfo: address information for use with TCP/IP
                // res: linked list, one or more addressinfo
                const auto ecode = getaddrinfo(ip.c_str(), std::to_string(config.port).c_str(), &hints, &res);
                ASSERT(!ecode, "getaddrinfo() failed. errno:" + std::string(gai_strerror(ecode)));
                int socket_fd = -1;
                int opt = 1;
                for (addrinfo *curr = res; curr != nullptr; curr = curr->ai_next)
                {
                    // create an unbound socket in a communications domain, and return a file descriptor that can be used in later function calls that operate on sockets.
                    // ipv4, stream/dram, tcp/udp
                    socket_fd = socket(curr->ai_family, curr->ai_socktype, curr->ai_protocol);
                    ASSERT(socket_fd != -1, "socket() failed. errno:" + std::string(strerror(errno)));
                    ASSERT(setSocketNonBlocking(socket_fd), "setNonBlocking() failed. errno:" + std::string(strerror(errno)));

                    // only applies to Tcp
                    if (!config.isUdp)
                    {
                        disableNagleAlgo(socket_fd);
                    }

                    if (!config.isListening)
                    {
                        // client branch
                        // system call: connect
                        // initiate a connection on a socket
                        auto r = connect(socket_fd, curr->ai_addr, curr->ai_addrlen);
                        // Operation now in progress is normal for non-blocking socket
                        cout << std::string(strerror(errno)) << endl;
                        // ASSERT(r != -1,
                        //        "connect() failed. errno:" + std::string(strerror(errno)));
                    }
                    else
                    {
                        // server branch
                        // allow reuse address
                        // SOL_SOCKET: opt at socket level
                        ASSERT(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&opt), sizeof(opt)) == 0,
                               "setsockopt() failed. errno:" + std::string(strerror(errno)));
                        // socket address: ipv4
                        // assign a local socket address address to a socket
                        // {htonl(INADDR_ANY)}: any ip address
                        const sockaddr_in addr{AF_INET, htons(config.port), {htonl(INADDR_ANY)}, {}};
                        // reuse logic for udp are special ??
                        ASSERT(bind(socket_fd, config.isUdp ? reinterpret_cast<const struct sockaddr *>(&addr) : curr->ai_addr, sizeof(addr)) == 0,
                               "bind() failed. errno:%" + std::string(strerror(errno)));

                        // listen for incoming TCP connections.only to stream sockets
                        // max queue size that server accpts.
                        // ECONNREFUSED
                        if (!config.isUdp)
                        {
                            ASSERT(listen(socket_fd, TcpServerBacklogSize) == 0, "Tcp Server listen() failed. errno:" + std::string(strerror(errno)));
                        }
                    }

                    if (config.useSoftwareTimestamp)
                    {
                        ASSERT(setSOTimestamp(socket_fd), "setSOTimestamp() failed. errno:" + std::string(strerror(errno)));
                    }
                }

                return socket_fd;
            }

        private:
            static constexpr size_t TcpServerBacklogSize = 16;
            // socket file descriptor
            int _fd{-1};
        };
    }
};