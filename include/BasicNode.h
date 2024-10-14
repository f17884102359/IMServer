#ifndef _BASICNODE_H
#define _BASICNODE_H

#include "Socket.h"
#include "IPAddress.h"
#include "Config.h"

class BasicNode {
    public:
        BasicNode(): _id(-1), _sock(NULL), _addr(NULL) {};
        ~BasicNode();
        bool NewSock(int domain, int type, int protocol);
        bool NewAddr(uint16_t protocol, const std::string ip, uint16_t port);
        Socket *Sock() {return _sock;}
        IPAddress *Addr() {return _addr;}
        int ID() {return _id;}
        void SetID(int id) {_id = id;}
    private:
        int _id;
        Socket *_sock;
        IPAddress *_addr;
};

#endif