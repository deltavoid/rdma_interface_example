#ifndef CQ_H
#define CQ_H

#include <queue>

#include "epoll_handler.h"


class CompletionEvent {
public:

    int id;

};


class CompletionQueue : public EpollHandler {
public:

    int event_fd;
    std::queue<CompletionEvent> que;


    CompletionQueue();
    virtual ~CompletionQueue();

    virtual int handle(uint32_t event);    


    int put_event(CompletionEvent& event);

};



#endif // CQ_H
