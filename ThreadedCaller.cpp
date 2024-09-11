#include "ThreadedCaller.h"
#include "ActualFunction.h"
#include "ServiceInterface.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>

double ThreadedCaller::runTest(int threadCount, uint64_t iterations) {
    using namespace std;
    using namespace std::chrono;
    typedef high_resolution_clock Clock;
    auto startTimeNanos = Clock::now();
    thread threads[threadCount];
    uint64_t results[threadCount];
    uint64_t result = 0;
    for (int threadIndex = 0; threadIndex < threadCount; threadIndex++) {
        // Create and start a thread with lambda
        threads[threadIndex] = thread([this, iterations, threadIndex, threadCount, &results] {
            uint64_t result = 0;
            for (uint64_t i = threadIndex; i < iterations; i += threadCount) {
                result += service->compute(i);
            }
            results[threadIndex] = result;
        });
    }
    for (int i = 0; i < threadCount; i++) {
        threads[i].join();
        result += results[i];
    }
    auto stopTimeNanos = Clock::now();
 //   cout << "Result: " << result << endl;
    return duration_cast<nanoseconds>(stopTimeNanos - startTimeNanos).count() / 1000000000.0;
}

ThreadedCaller::ThreadedCaller(ServiceInterface *service) {
    this->service = service;
}
