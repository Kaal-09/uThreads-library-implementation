#include "uthread/uthread.h"
#include <iostream>
#include "scheduler.h"
#include <vector>

static Schedular schedular;
static std::vector <TCB> tcbs;
static std::uint64_t next_id = 1;

const size_t STACK_SIZE = 64 * 1024;

static void thread_trampoline(TCB* tcb){
    tcb->func(tcb->arg);
    schedular.exit_current();
}

namespace uthread {
    void init() {
        tcbs.reserve(16);
        
        TCB main_thread;
        main_thread.id = 0;
        main_thread.state = ThreadState::RUNNING;
        main_thread.func = nullptr;
        main_thread.arg = nullptr;
        main_thread.stack = nullptr;
        main_thread.stack_size = 0;
        
        tcbs.push_back(main_thread);
        TCB* main_thread_ptr = &tcbs[0];
        getcontext(&main_thread_ptr->context);
        
        schedular.add_thread(&tcbs[0]);
        schedular.init();
    }

    thread_t create(void (*func)(void*), void* arg) {
        tcbs.emplace_back();
        TCB& t = tcbs.back();

        t.id = next_id++;
        t.state = ThreadState::READY;
        t.func = func;
        t.arg = arg;
        t.started = false;

        t.stack_size = STACK_SIZE;
        t.stack = malloc(STACK_SIZE);

        getcontext(&t.context);

        t.context.uc_stack.ss_sp = t.stack;
        t.context.uc_stack.ss_size = t.stack_size;
        t.context.uc_link = nullptr;

        makecontext(
            &t.context,
            (void(*)())thread_trampoline,
            1,
            &t
        );
        schedular.add_thread(&t);

        return t.id;
    }
    void yield() {
        schedular.yield();
    }

    void exit() {
        schedular.exit_current();
    }
    void check_preemption() {
        if (schedular.should_preempt()) {
            schedular.clear_preempt_flag();
            schedular.yield();
        }
    }
}