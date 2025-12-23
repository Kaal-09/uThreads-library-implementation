#include "uthread/uthread.h"
#include <iostream>

namespace uthread {
    void init() {
        std::cout << "[uthread] init called\n";
    }
    thread_t create(void (*func)(void*), void* arg) {
        std::cout << "[uthread] create called\n";
        // TEMPORARY: just call the function directly
        func(arg);
        return 0;
    }
    void yield() {
        std::cout << "[uthread] yield called\n";
    }

    void exit() {
        std::cout << "[uthread] exit called\n";
    }
}