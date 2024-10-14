#ifndef _IP_H
#define _IP_H

#include <string>
#include <IPAddress.h>
extern "C" {
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
}

typedef struct Addr {
    union {
            sockaddr_in ip4;
            sockaddr_in6 ip6;
        } addr;
        bool isipv6;
} Addr_t;

class IPAddress {
    public:
        IPAddress(uint16_t protocol, std::string ipstr, uint16_t port);
        ~IPAddress();
        Addr_t *Addr() {return &_addr;}
        bool IsIPv6() {return _addr.isipv6;}
        uint16_t Port() {
            if (IsIPv6()) {
                return _addr.addr.ip6.sin6_port;
            }
            return _addr.addr.ip4.sin_port;
        }

    private:
        Addr_t _addr;
};

#endif