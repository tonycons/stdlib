#pragma once

#include "system.hh"  // IWYU pragma: keep
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


namespace cm::net {


enum class Protocol {
    TCP,
    UDP,
};


struct ServerSocket
{
private:
    int _fd;

    ServerSocket(int fd)
        : _fd(fd)
    {}

public:
    ~ServerSocket() { ::close(_fd); }


    ///
    /// Create a server socket listening on a given port.
    /// @param port The port to use
    /// @param protocol The underlying data protocol
    ///
    static Optional<ServerSocket> open(int port, Protocol protocol)
    {
        int fd = ::socket(
            AF_INET,
            [&] {
                switch (protocol) {
                case Protocol::TCP: return SOCK_STREAM;
                case Protocol::UDP: return SOCK_DGRAM;
                default: return SOCK_DGRAM;  // NOLINT
                };
            }(),
            0);
        if (fd < 0) {
            return None;
        }
        sockaddr_in server_address{};
        server_address.sin_family = AF_INET;  // set to AF_INET for TCP and UDP
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        server_address.sin_port = htons(port);

        // ugh who wrote this terrible api requiring an ugly pointer cast
        // (violates strict aliasing btw)
        if (::bind(fd, reinterpret_cast<sockaddr const*>(&server_address), sizeof(sockaddr_in)) < 0) {
            ::close(fd);
            return None;
        }

        return ServerSocket(fd);
    }

    ///
    /// Blocks the current thread to listen for client connections.
    /// Whenever a client connection occurs, the provided callback is executed.
    ///
    bool listen(IsCallableWith<ServerSocket*> auto&& callback)
    {
        if (::listen(_fd, SOMAXCONN) < 0) {
            ::close(_fd);
            return false;
        }
        while (true) {}
    }
};


}  // namespace cm::net
