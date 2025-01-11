#ifndef _SOCKET_H
#define _SOCKET_H

#include <IPAddress.h>
extern "C" {
#include <sys/socket.h>
#include <unistd.h>
}

class Socket {
public:
    Socket(int domain, int type, int protocol);
    Socket(int sock): _sock(sock) {};
    ~Socket();

    int Sock() {return _sock;}
    int Bind(IPAddress& addr);
    int Listen(int backlog = 5);
    Socket *Accept();
    int Connect(IPAddress& addr);
    ssize_t Send(const void* buffer, size_t length);
    ssize_t Recv(void* buffer, size_t length);
    int SetNonBlocking();
    int SetReuseAddr();
    int SetReusePort();

private:
    int _sock;
};

#endif