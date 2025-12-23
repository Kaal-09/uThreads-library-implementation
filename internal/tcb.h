#pragma once

#include <cstdint>
#include <csetjmp>

enum class ThreadState {
    READY,
    RUNNING,
    FINISHED
};

struct TCB {
    std::uint64_t id;
    ThreadState state;
    jmp_buf context;
    void (*func)(void*);
    void* arg;
};
