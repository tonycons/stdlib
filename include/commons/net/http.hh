#pragma once
#include <commons/system.hh>
#include <commons/datastructs.hh>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


using namespace cm;

struct HttpServer;


struct HttpResponse
{
    static auto custom(int code, StringRef mimeType, StringRef body) -> HttpResponse { return {code, mimeType, body}; }

    static auto json(StringRef body) { return custom(200, "application/json", body); }

    void send(HttpServer* server);

    int code;
    StringRef mimeType;
    StringRef body;
};


struct HttpRequest
{
    enum Type {
        GET,
        POST
    } type{};


    using ParamKey = FixedString<64>;
    using ParamValue = FixedString<256>;
    using Handler = HttpResponse (*)(HttpRequest const&);


    String route{};
    FixedString<16> protocol{};
    FixedMap<ParamKey, ParamValue, 128> params{};
    ScratchAllocator<ParamKey, 128> paramKeyAllocator{};
    ScratchAllocator<ParamValue, 128> paramValueAllocator{};
};


struct HttpServer
{
    int fd{};
    int epoll_fd{};
    pthread_t thread_id{};
    HttpRequest::Handler handler{};
    int port{};

    HttpServer(int port);
    ~HttpServer();

    ScratchAllocator<char, 1024> responseAllocator;

private:
    static void* thread(void* param);


    void _send(int client_fd, HttpResponse const& response)
    {
        auto const contentLengthStr = FixedString<32>::cformat("%zu", response.body.length());

        // all the strings need to be combined into one contiguous string response
        // calculate its size
        auto const responseSize =
            sizeof(
                "HTTP/1.1 \r\nContent-Type: \r\nContent-Length: \r\nConnection: close\r\n\r\n")  // base header length
            + 3  // length of code in header
            + response.mimeType.length() + contentLengthStr.length() + response.body.length();

        // allocate
        auto& allocator = responseSize < 1024 ? static_cast<Arena<char>&>(responseAllocator) :
                                                static_cast<Arena<char>&>(mainAllocator<char>);
        auto mem = Mem<char>(responseSize, allocator);
        auto buf = StringRef(mem.data(), mem.length());

        buf.nprintf(
            mem.length() - 1, "HTTP/1.1 %03d\r\nContent-Type: %s\r\nContent-Length: %s\r\nConnection: close\r\n\r\n%s",
            response.code, response.mimeType.cstr(), response.body.length(), response.body.cstr());

        ::send(client_fd, buf.data(), buf.length(), 0);
        ::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
        ::close(client_fd);
    }

    ///
    /// Parses an HTTP request. Return None for a malformed request
    ///
    static auto httpRequest(char const* input) -> std::optional<HttpRequest>;
};
