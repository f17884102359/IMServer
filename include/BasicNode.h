#ifndef _BASICNODE_H
#define _BASICNODE_H

#include <map>
#include <utility>

#include "Socket.h"
#include "IPAddress.h"
#include "Config.h"

class BasicNode;
extern BasicNode *Listener;
extern std::map<int, BasicNode *> g_node_map;

#define LISTENER_ID 0

class BasicNode {
    public:
        BasicNode(): _id(-1), _sock(NULL), _addr(NULL) {};
        BasicNode(int id, Socket *sock, IPAddress *addr): _id(id), _sock(sock), _addr(addr) {};
        ~BasicNode();
        bool NewSock(int domain, int type, int protocol);
        bool NewSock(int sock) {_sock = new Socket(sock); return _sock != NULL;}
        bool NewAddr(uint16_t protocol, const std::string ip, uint16_t port);
        bool NewAddr(uint32_t ip, uint16_t port) {_addr = new IPAddress(ip, port); return _addr != NULL;}
        Socket *Sock() {return _sock;}
        IPAddress *Addr() {return _addr;}
        int ID() {return _id;}
        void SetID(int id) {_id = id;}
        bool IsListener() {return _id == LISTENER_ID;}
        void ProcessEvent();
        int Accept();
    private:
        int _id;
        Socket *_sock;
        IPAddress *_addr;
};

#endif