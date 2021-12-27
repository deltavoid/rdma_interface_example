#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include <thread>
#include <memory>

#include "io_context.h"
#include "cq.h"

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

    std::thread thread1 = std::thread(
        [&io_context1]() {  io_context1.run();});


    for (int i = 0; i < 3; i++)
    {
        CompletionEvent cq_event;
        cq_event._id = i;

        cq1.put_event(cq_event);
        sleep(1);
    }

    

    thread1.join();
    return 0;
}