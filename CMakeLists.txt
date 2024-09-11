cmake_minimum_required(VERSION 3.29)
project(uservicelab)

set(CMAKE_CXX_STANDARD 23)

add_executable(uservicelab main.cpp
        ThreadedCaller.cpp
        ThreadedCaller.h
        ActualFunction.cpp
        ActualFunction.h
        LocalThreaded/ThreadedService.cpp
        LocalThreaded/ThreadedService.h
        Network/Server.cpp
        Network/Server.h
        Network/Client.cpp
        Network/Client.h
        Network/SocketIO.cpp
        Network/SocketIO.h
        ConcurrentQueue.cpp
        ConcurrentQueue.h
)
