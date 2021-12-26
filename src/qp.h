#ifndef QP_H
#define QP_H


#include <queue>

#include "epoll_handler.h"



class QpRequest {
public:
    int id;


};


class CompletionQeuue;

class QpRequestQueue : public EpollHandler {
public:
    int event_fd;
    std::queue<QpRequest> que;

    CompletionQueue* cq;

    QpRequestQueue();
    virtual ~QpRequestQueue();

    virtual int handle(uint32_t event);

    int put_request(QpRequest& request);

};



#endif // QP_H
