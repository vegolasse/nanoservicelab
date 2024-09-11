#include <iostream>
#include "ThreadedCaller.h"
#include "ActualFunction.h"
#include "LocalThreaded/ThreadedService.h"
#include "Network/Server.h"
#include "Network/Client.h"

int main() {
    int iterations = 1000 * 100;
    {
        int callerThreads = 12;
        ThreadedCaller caller(new ActualFunction());
        double runtimeSeconds = caller.runTest(callerThreads, iterations);
        // calculate time per iteration in nanoseconds
        uint64_t timePerIteration = runtimeSeconds / iterations * 1000000000;
        std::cout << "Direct (" << callerThreads << " threads); "
                << timePerIteration << "; nanoseconds/call; " << runtimeSeconds << "; seconds total;"
                << std::endl;
    }
    {
        int callerThreads = 12;
        int workerThreads = 12;
        ThreadedCaller caller(new ThreadedService(workerThreads));
        double runtimeSeconds = caller.runTest(callerThreads, iterations);
        uint64_t timePerIteration = runtimeSeconds / iterations * 1000000000;
        std::cout << "Threaded (" << callerThreads << " threads, " << workerThreads << " workers); "
                  << timePerIteration << "; nanoseconds/call; " << runtimeSeconds << "; seconds total;"
                  << std::endl;
    }
    {
        int callerThreads = 128;
        int serverThreads = 128;
        Server server(serverThreads, 8080);
        {
            Client client(8080, serverThreads);
            ThreadedCaller caller(&client);
            double runtimeSeconds = caller.runTest(callerThreads, iterations);
            // std::cout << "Network: " << runtimeSeconds << " seconds" << std::endl;
            uint64_t timePerIteration = runtimeSeconds / iterations * 1000000000;
            std::cout << "TCP 127.0.0.1 (" << callerThreads << " threads, " << serverThreads << " workers); "
                      << timePerIteration << "; nanoseconds/call; " << runtimeSeconds << "; seconds total;"
                      << std::endl;
        }

    }

    return 0;
}
