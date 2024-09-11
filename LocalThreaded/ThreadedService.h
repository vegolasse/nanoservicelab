//
// Created by Lars Sjödin on 2024-08-25.
//

#ifndef USERVICELAB_THREADEDSERVICE_H
#define USERVICELAB_THREADEDSERVICE_H

#include <cstdint>
#include <thread>
#include <semaphore>
#include <queue>
#include "../ServiceInterface.h"
#include "../ConcurrentQueue.h"

class CallData {
public:
    CallData();

    uint64_t number;
    uint64_t result;
    std::counting_semaphore<1> sem = std::counting_semaphore<1>(0);
};


class ThreadedService : public ServiceInterface {
public:
    ThreadedService(int workerThreads);
    ~ThreadedService();
    uint64_t compute(uint64_t number) ;

private:
    ConcurrentQueue<CallData*> callQueue;
    std::vector<std::thread> workers;
};


#endif //USERVICELAB_THREADEDSERVICE_H
