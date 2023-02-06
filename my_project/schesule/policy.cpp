#include <iostream>
#include <thread>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    auto pid = getpid();
    struct sched_param para;
    para.sched_priority = 5;
    int err = sched_setscheduler(pid,SCHED_RR,&para);
    if(err == -1){
        std::cout << "error is " << strerror(errno) << std::endl;
    }
    sleep(10);
    auto policy = sched_getscheduler(pid);
    std::cout << policy << std::endl;
    return 0;
}
