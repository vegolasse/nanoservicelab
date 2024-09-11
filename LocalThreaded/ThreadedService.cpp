#include "ThreadedService.h"
#include "../ActualFunction.h"
#include <chrono>
#include <iostream>
#include <semaphore>


ThreadedService::ThreadedService(int workerThreads) {
    //Create worker threads
    for (int i = 0; i < workerThreads; i++) {
        ActualFunction actualFunction;
        workers.push_back(std::thread([this, &actualFunction] {
            while (true) {
                CallData* callData = callQueue.pop();
                if (callData == NULL) {
                    break;
                }
                callData->result = actualFunction.compute(callData->number);
                callData->sem.release();
            }
        }));
    }

}

ThreadedService::~ThreadedService() {
    for (int i = 0; i < workers.size(); i++) {
        callQueue.push(NULL);
    }
    for (int i = 0; i < workers.size(); i++) {
        workers[i].join();
    }
}

uint64_t ThreadedService::compute(uint64_t number) {
    CallData* callData = new CallData();
    callData->number = number;
    callQueue.push(callData);
    callData->sem.acquire();
    uint64_t result = callData->result;
    delete callData;
    return result;
}

CallData::CallData() {

}
