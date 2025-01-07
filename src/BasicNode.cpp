#include "BasicNode.h"
#include "Log.h"
#include <sys/epoll.h>

extern int epoll_fd;

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

int BasicNode::Accept()
{
    LOG(Info) << "Accept";
    struct sockaddr_in client_addr4;
    socklen_t client_addr_len = sizeof(client_addr4);
    Socket *Client_Sock = NULL;
    Addr_t *Client_Addr = NULL;
    BasicNode *client = NULL;
    epoll_event ev;

    int client_sock = accept(_sock->Sock(), (struct sockaddr *)&client_addr4, &client_addr_len);
    int client_id = g_node_map.size();
    

    if (client_sock == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;
        }
        goto err;
    }

    if (client_id >= MAX_CLIENTS) {
        LOG(Error) << "Too many clients";
        goto err;
    }

    Client_Sock = new Socket(client_sock);
    if (Client_Sock == NULL) {
        LOG(Error) << "new client socket failed";
        goto err;
    }

    client = new BasicNode();
    if (client == NULL) {
        LOG(Error) << "new client failed";
        goto err;
    }

    client->SetID(client_id);
    if (client->NewSock(client_sock) == false) {
        LOG(Error) << "new client socket failed";
        goto err;
    }

    if (client->Sock()->SetNonBlocking() == -1) {
        LOG(Error) << "client set non-blocking failed";
        goto err;
    }

    if (client->NewAddr(client_addr4.sin_addr.s_addr, client_addr4.sin_port) == false) {
        LOG(Error) << "new client address failed";
        goto err;
    }

    g_node_map.insert(std::make_pair(client_id, client));
    if (g_node_map.find(client_id) == g_node_map.end()) {
        LOG(Error) << "client insert failed";
        goto err;
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = client;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client->Sock()->Sock(), &ev) == -1) {
        LOG(Error) << "epoll_ctl add client failed";
        goto err;
    }

    return 0;
    

err:
    if (client_sock != -1) {
        close(client_sock);
    }

    if (Client_Sock) {
        delete Client_Sock;
    }

    if (Client_Addr) {
        delete Client_Addr;
    }

    if (client) {
        delete client;
    }

    return -1;
}

void BasicNode::ProcessEvent()
{
    LOG(Info) << "ProcessEvent";
    char buf[MAX_MSG_LEN];
    if (IsListener()) {
        if (Accept() == -1) {
            LOG(Error) << "accept failed";
        }
        return;
    }

    int n = _sock->Recv(buf, MAX_MSG_LEN);
    if (n == 0) {
        LOG(Info) << "client close";
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, _sock->Sock(), NULL);
        g_node_map.erase(_id);
        delete this;
        return;
    } else if (n < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;
        }

        LOG(Error) << "recv failed";
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, _sock->Sock(), NULL);
        g_node_map.erase(_id);
        delete this;
        return;
    } else {
        LOG(Info) << "recv: " << buf;
    }
    
    return;
}