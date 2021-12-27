#ifndef IO_CONTEXT_H
#define IO_CONTEXT_H

#include "epoll_handler.h"

class IOContext {
public:
    int epoll_fd;
    int running;

    IOContext();
    ~IOContext();

    void run();

    int add_handler(int fd, uint32_t events, EpollHandler* handler);

};


#endif // IO_CONTEXT_H
