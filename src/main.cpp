#include "BasicNode.h"

int main()
{
    BasicNode *listener = new BasicNode();
    if (listener == NULL) {
        return -1;
    }
    if (listener->NewAddr(AF_INET, "192.168.80.41", 8888) == false) {
        return -1;
    }

    if (listener->NewSock(AF_INET, SOCK_STREAM, 0) == false) {
        return -1;
    }

    if (listener->Sock()->Bind(*listener->Addr()) == -1) {
        return -1;
    }

    if (listener->Sock()->Listen() == -1) {
        return -1;
    }

    while (1) {
        sleep(1);
    }

    delete listener;

    return 0;
}