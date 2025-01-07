#ifndef _CONFIG_H
#define _CONFIG_H

#define MAX_CONNECTS    1000
#define MAX_LEN         1024
#define MAX_EVENTS      100
#define MAX_THREADS     10
#define MAX_CLIENTS     1000
#define MAX_MSG_LEN     1024
#define MAX_NAME_LEN    32
#define MAX_PWD_LEN     32
#define MAX_GROUP_NUM   32
#define MAX_GROUP_MEMBER 32
#define MAX_GROUP_NAME  32
#define MAX_GROUP_DESC  128
#define BASEPATH        "/root/code/IMServer/IMServer/"
#define LOGPATH         BASEPATH"log/"
#define LOGFILE         LOGPATH"server.log"
#define EPOLL_TIMEOUT   1000

#endif