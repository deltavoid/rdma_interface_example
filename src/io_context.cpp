#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>

#include "io_context.h"
#include "epoll_handler.h"




IOContext::IOContext()
{
    epoll_fd = epoll_create1(0);

    running = true;

}



IOContext::~IOContext()
{
    close(epoll_fd);
}




void IOContext::run()
{
    printf("IOContext::run: 1\n");
    const int internal_event_num = 128;
    struct epoll_event internal_events[internal_event_num];


    while (running)
    {
        int num = epoll_wait(epoll_fd, internal_events, internal_event_num, 0);
        if  (num < 0)
            perror("epoll wait error");

        for (int i = 0; i < num; i++)
        {
            struct epoll_event& ev = internal_events[i];
            EpollHandler* handler = (EpollHandler*)ev.data.ptr;
            
            int ret = handler->handle(ev.events);
            if  (ret < 0)
            {
                printf("handler failed\n");
                delete handler;
            }

        }
    }

}


int IOContext::add_handler(int fd, uint32_t events, EpollHandler* handler)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = handler;

    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if  (ret < 0)
        return -1;

    return 0;
}
