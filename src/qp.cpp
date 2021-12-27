#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include "io_context.h"
#include "qp.h"
#include "cq.h"






QpRequestQueue::QpRequestQueue(IOContext* io_context)
{
    _event_fd = eventfd(0, 0);

    pthread_spin_init(&_que_lock, 0);

    _io_context = io_context;

    _io_context->add_handler(_event_fd, EPOLLIN, this);

}

QpRequestQueue::~QpRequestQueue()
{
    close(_event_fd);

    pthread_spin_destroy(&_que_lock);
}


int QpRequestQueue::handle(uint32_t event)
{
    acknowledge();

    bool have_event;
    do
    {
        have_event = false;
        QpRequest request;

        pthread_spin_lock(&_que_lock);

        if  (!_que.empty())
        {
            request = _que.front();
            _que.pop();
            have_event = true;
        }

        pthread_spin_unlock(&_que_lock);

        if  (have_event)
            handle_request(request);

    } while(have_event);
}

int QpRequestQueue::handle_request(QpRequest& request)
{
    printf("QpRequestQueue::handle_request: request id: %d\n", request._id);

    CompletionEvent cq_event;
    cq_event._id = request._id;

    _cq->put_event(cq_event);

    return 0;
}

int QpRequestQueue::put_request(QpRequest& request)
{
    pthread_spin_lock(&_que_lock);

    _que.push(request);

    pthread_spin_unlock(&_que_lock);

    notify();

    return 0;
}

void QpRequestQueue::notify()
{
    uint64_t val = 1;
    int ret = write(_event_fd, &val, sizeof(val));
    if  (ret < 0)
        perror("write event_fd error");
}

void QpRequestQueue::acknowledge()
{
    uint64_t val = 0;
    int ret = read(_event_fd, &val, sizeof(val));
    if  (ret < 0)
        perror("read event fd error");
    
}
