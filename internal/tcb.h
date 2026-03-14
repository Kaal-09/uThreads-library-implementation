#pragma once

#include <cstdint>
#include <csetjmp>
#include <ucontext.h>

enum class ThreadState {
    READY,
    RUNNING,
    FINISHED
};

struct TCB {
    std::uint64_t id;
    ThreadState state;
    ucontext_t context;
    void (*func)(void*);
    void* arg;
    bool started;
    void* stack;
    size_t stack_size;
};
