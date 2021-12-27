#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include <thread>
#include <memory>

#include "io_context.h"
#include "cq.h"
#include "qp.h"

int main(int argc, char** argv, char** env)
{
    // printf("env:\n");
    // for (int i = 0; env[i] != NULL; i++)
    //     printf("%s\n", env[i]);
    // printf("\n");

    // printf("arg:\n");
    // for (int i = 0; i < argc; i++)
    //     printf("%s\n", argv[i]);

    IOContext io_context1;

    CompletionQueue cq1(&io_context1);

    std::thread thread1(
        [&io_context1]() {  io_context1.run();});


    // for (int i = 0; i < 3; i++)
    // {
    //     CompletionEvent cq_event;
    //     cq_event._id = i;

    //     cq1.put_event(cq_event);
    //     sleep(1);
    // }

    IOContext io_context2;

    QpRequestQueue qp1(&io_context2, &cq1);

    std::thread thread2(
        [&io_context2]() {  io_context2.run();});


    QpRequest request;
    request._id = 1;

    qp1.put_request(request);   



    thread1.join();
    thread2.join();
    return 0;
}