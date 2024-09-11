#pragma once
#include <cstdint>
#include "ServiceInterface.h"

class ActualFunction : public ServiceInterface {
public:
    __attribute__((noinline)) uint64_t compute(uint64_t number) ;
};
