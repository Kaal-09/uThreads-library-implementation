#pragma once

#include <vector>
#include "tcb.h"

class Schedular {
    public:
        void init();
        void add_thread(TCB* tcb);
        void yield();
        void block_current();
        void unblock(TCB* tcb);
        void exit_current();
        void disable_preemption();
        void enable_preemption();
        TCB* get_current_tcb();
        bool should_preempt();
        void clear_preempt_flag();

    private:
        std::vector<TCB*> threads;
        int current = -1;
};