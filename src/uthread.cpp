#include "uthread/uthread.h"
#include <iostream>
#include "scheduler.h"
#include <vector>

static Schedular schedular;
static std::vector <TCB> tcbs;
static std::uint64_t next_id = 1;

namespace uthread {
    void init() {
        tcbs.reserve(16);
        
        TCB main_thread;
        main_thread.id = 0;
        main_thread.state = ThreadState::RUNNING;
        main_thread.func = nullptr;
        main_thread.arg = nullptr;

        tcbs.push_back(main_thread);
        schedular.add_thread(&tcbs[0]);
        schedular.init();
    }

    thread_t create(void (*func)(void*), void* arg) {
        TCB t;
        t.id = next_id++;
        t.state = ThreadState::READY;
        t.func = func;
        t.arg = arg;

        tcbs.push_back(t);
        schedular.add_thread(&tcbs.back());

        return t.id;
    }
    void yield() {
        schedular.yield();
    }

    void exit() {
        schedular.exit_current();
    }
}