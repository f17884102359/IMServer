#include "IPAddress.h"

IPAddress::IPAddress(uint16_t protocol, const std::string ip, uint16_t port)
{
    memset(&_addr, 0, sizeof(_addr));

    if (ip.find(":") == std::string::npos) {
        /* IPv4 */
        _addr.isipv6 = false;
        _addr.addr.ip4.sin_family = AF_INET;
        _addr.addr.ip4.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &_addr.addr.ip4.sin_addr);
    } else {
        /* IPv6 */
        _addr.isipv6 = true;
        _addr.addr.ip6.sin6_family = AF_INET6;
        _addr.addr.ip6.sin6_port = htons(port);
        inet_pton(AF_INET6, ip.c_str(), &_addr.addr.ip6.sin6_addr);
    }
}

IPAddress::~IPAddress()
{
}