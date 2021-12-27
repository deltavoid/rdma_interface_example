
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

#include "cq.h"
#include "io_context.h"




CompletionQueue::CompletionQueue(IOContext* io_context)
{
    _io_context = io_context;

    pthread_spin_init(&_que_lock, 0);

    _event_fd = eventfd(0, 0);

    _io_context->add_handler(_event_fd, EPOLLIN, this);


}

CompletionQueue::~CompletionQueue() 
{
    close(_event_fd);

    pthread_spin_destroy(&_que_lock);

}


int CompletionQueue::handle(uint32_t event)
{
    acknowledge();

    bool have_event;
    do
    {
        have_event = false;
        CompletionEvent event;
        pthread_spin_lock(&_que_lock);

        if  (!_que.empty())
        {
            event = _que.front();
            _que.pop();
            have_event = true;
        }

        pthread_spin_unlock(&_que_lock);

        
        printf("CompletionQueue::handle, completion_event: %d\n", event._id);


    } while(have_event);

}

int CompletionQueue::put_event(CompletionEvent& event)
{
    // int ret = -1;

    pthread_spin_lock(&_que_lock);

    _que.push(event);

    pthread_spin_unlock(&_que_lock);

    notify();

    return 0;
}

void CompletionQueue::notify()
{
    uint64_t val = 1;
    int ret = write(_event_fd, &val, sizeof(val));
    if  (ret < 0)
        perror("write event_fd error");

}

void CompletionQueue::acknowledge()
{
    uint64_t val = 0;
    int ret = read(_event_fd, &val, sizeof(val));
    if  (ret < 0)
        perror("read event fd error");
    
}