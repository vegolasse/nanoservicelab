#pragma once

#include <semaphore>
#include <queue>
#include <iostream>
#include <atomic>

template<class StoredType>
class ConcurrentQueue {
public:
    ConcurrentQueue() : head(0), tail(0), semaphore(0) {}

    void push(StoredType callData) {
        int indexToStore = head.fetch_add(1, std::memory_order_release) % SIZE;
        queue[indexToStore] = callData;
        std::atomic_thread_fence(std::memory_order_seq_cst);
        semaphore.release();
    }

    StoredType pop() {
        semaphore.acquire();
        int indexToGet = tail.fetch_add(1) % SIZE;
        std::atomic_thread_fence(std::memory_order_seq_cst);
        StoredType ReturnValue;
        while ((ReturnValue = queue[indexToGet]) == NULL) {
        }
        return ReturnValue;
    }

private:
    static const int SIZE = 262144;
    std::array<std::atomic<StoredType>, SIZE> queue;  // Use std::array for fixed size
    std::atomic<int> head;
    std::atomic<int> tail;
    std::counting_semaphore<512> semaphore;  // Use semaphore for blocking
};
