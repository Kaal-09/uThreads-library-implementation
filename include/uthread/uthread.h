#pragma once

#include <cstddef>

namespace uthread {
    using thread_t = std::size_t;
    
    void init();
    thread_t create(void (*func)(void*), void* arg);
    void yield();
    void join(thread_t id);
    void exit();
}