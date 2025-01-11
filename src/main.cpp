extern "C" {
#include <sys/epoll.h>
#include <fcntl.h>
}
#include "BasicNode.h"
#include "Log.h"
#include "Config.h"

BasicNode *Listener;
std::map<int, BasicNode *> g_node_map;
int epoll_fd;
struct epoll_event events[MAX_EVENTS];

int ServerInit()
{
    Listener = new BasicNode();
    if (Listener == NULL) {
        LOG(Error) << "new Listener failed";
        return -1;
    }

    if (Listener->NewAddr(AF_INET, "192.168.80.41", 8888) == false) {
        LOG(Error) << "new Listener address failed";
        return -1;
    }

    if (Listener->NewSock(AF_INET, SOCK_STREAM, 0) == false) {
        LOG(Error) << "new Listener socket failed";
        return -1;
    }

    if (Listener->Sock()->SetNonBlocking() == -1) {
        LOG(Error) << "Listener set non-blocking failed";
        return -1;
    }

    if (Listener->Sock()->SetReuseAddr() == -1) {
        LOG(Error) << "Listener set reuse address failed";
        return -1;
    }

    if (Listener->Sock()->SetReusePort() == -1) {
        LOG(Error) << "Listener set reuse port failed";
        return -1;
    }

    if (Listener->Sock()->Bind(*Listener->Addr()) == -1) {
        LOG(Error) << "Listener bind failed";
        return -1;
    }

    if (Listener->Sock()->Listen() == -1) {
        LOG(Error) << "Listener listen failed";
        return -1;
    }

    Listener->SetID(LISTENER_ID);

    return 0;
}

int main()
{
    Log::setOutputFile(LOGFILE);

    if (ServerInit() != 0) {
       return -1;
    }

    LOG(Info) << "Server start";

    epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if (epoll_fd == -1) {
        LOG(Error) << "epoll_create1 failed";
        return -1;
    }

    struct epoll_event event{};
    event.events = EPOLLIN;
    event.data.ptr = Listener;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Listener->Sock()->Sock(), &event) == -1) {
        LOG(Error) << "epoll_ctl failed";
        return -1;
    }

    LOG(Info) << "epoll_ctl success";

    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, EPOLL_TIMEOUT);
        if (nfds == -1) {
            LOG(Error) << "epoll_wait failed";
            return -1;
        }

        LOG(Debug) << "epoll_wait return " << nfds;

        for (int i = 0; i < nfds; i++) {
            BasicNode *node = (BasicNode *)events[i].data.ptr;
            node->ProcessEvent();
        }

        usleep(1000);
    }

    delete Listener;

    return 0;
}