#ifndef CQ_H
#define CQ_H

#include <pthread.h>

#include <queue>

#include "epoll_handler.h"

class CompletionEvent {
public:

    int _id;

};


class IOContext;

class CompletionQueue : public EpollHandler {
public:

    IOContext* _io_context;
    int _event_fd;

    pthread_spinlock_t _que_lock;
    std::queue<CompletionEvent> _que;




    CompletionQueue(IOContext* io_context);
    virtual ~CompletionQueue();

    virtual int handle(uint32_t event);

    int handle_event(CompletionEvent& event);


    int put_event(CompletionEvent& event);

    void notify();
    void acknowledge();

};



#endif // CQ_H
