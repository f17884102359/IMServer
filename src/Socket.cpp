#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include "Log.h"
#include "Socket.h"

// 构造函数：创建套接字
Socket::Socket(int domain, int type, int protocol) {
    _sock = socket(domain, type, protocol);
    if (_sock < 0) {
        std::cerr << "Socket creation failed." << std::endl;
    }
}

// 析构函数：关闭套接字
Socket::~Socket() {
    close(_sock);
}

// Bind函数：将套接字绑定到特定的IP地址和端口
int Socket::Bind(IPAddress& addr) {
    if (addr.Addr()->isipv6) {
        return bind(_sock, (struct sockaddr*)&addr.Addr()->addr.ip6, sizeof(addr.Addr()->addr.ip6));
    } else {
        return bind(_sock, (struct sockaddr*)&addr.Addr()->addr.ip4, sizeof(addr.Addr()->addr.ip4));
    }
}

// Listen函数：监听传入的连接
int Socket::Listen(int backlog) {
    return listen(_sock, backlog);
}

int Socket::SetNonBlocking() {
    int flags = fcntl(_sock, F_GETFL, 0);
    if (flags == -1) {
        LOG(Error) << "fcntl get failed";
        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(_sock, F_SETFL, flags) == -1) {
        LOG(Error) << "fcntl set failed";
        return -1;
    }

    return 0;
}

// Connect函数：客户端向服务器发起连接
int Socket::Connect(IPAddress& addr) {
    if (addr.Addr()->isipv6) {
        return connect(_sock, (struct sockaddr*)&addr.Addr()->addr.ip6, sizeof(addr.Addr()->addr.ip6));
    } else {
        return connect(_sock, (struct sockaddr*)&addr.Addr()->addr.ip4, sizeof(addr.Addr()->addr.ip4));
    }
}

// Send函数：发送数据
ssize_t Socket::Send(const void* buffer, size_t length) {
    return send(_sock, buffer, length, 0);
}

// Recv函数：接收数据
ssize_t Socket::Recv(void* buffer, size_t length) {
    return recv(_sock, buffer, length, 0);
}
