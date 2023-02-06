#include <iostream>
#include <thread>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    auto pid = getpid();
    struct sched_param para;
    para.sched_priority = 5;
    sched_setscheduler(pid,SCHED_BATCH,&para);
    auto policy = sched_getscheduler(pid);
    std::cout << policy << std::endl;
    return 0;
}
