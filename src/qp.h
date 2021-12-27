#ifndef QP_H
#define QP_H

#include <pthread.h>

#include <queue>

#include "epoll_handler.h"



class QpRequest {
public:
    int _id;


};


class IOContext;
class CompletionQueue;

class QpRequestQueue : public EpollHandler {
public:
    IOContext* _io_context;
    CompletionQueue* _cq;

    int _event_fd;

    pthread_spinlock_t _que_lock;
    std::queue<QpRequest> _que;

    
    QpRequestQueue(IOContext* io_context);
    virtual ~QpRequestQueue();

    virtual int handle(uint32_t event);

    int handle_request(QpRequest& request);

    int put_request(QpRequest& request);

    void notify();
    void acknowledge();

};



#endif // QP_H
