#include "BasicNode.h"

BasicNode::~BasicNode()
{
    if (_sock) {
        delete _sock;
    }

    if (_addr) {
        delete _addr;
    }
}

bool BasicNode::NewSock(int domain, int type, int protocol)
{
    _sock = new Socket(domain, type, protocol);
    if (_sock == NULL) {
        return false;
    }

    return true;
}
bool BasicNode::NewAddr(uint16_t protocol, const std::string ip, uint16_t port)
{
    _addr = new IPAddress(protocol, ip, port);
    if (_addr == NULL) {
        return false;
    }
    return true;
}