#pragma once

#include <cstdint>

class ServiceInterface {

public:
 virtual uint64_t compute(uint64_t number)  =0;
};

