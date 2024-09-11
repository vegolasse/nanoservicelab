#pragma once

#include <cstdint>
#include "ServiceInterface.h"

class ThreadedCaller {
public:
    ThreadedCaller(ServiceInterface* service);
    double runTest(int threadCount, uint64_t iterations);

    ServiceInterface *service;
};
