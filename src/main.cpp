#include <cstdio>
#include <cstdlib>

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

    CompletionEvent cq_event1;
    cq_event1._id = 1;

    cq1.put_event(cq_event1);
    

    thread1.join();
    return 0;
}