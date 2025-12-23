#pragma once

#include <vector>
#include "tcb.h"

class Schedular {
    public:
        void init();
        void add_thread(TCB* tcb);
        void yield();
        void exit_current();

    private:
        std::vector<TCB*> threads;
        int current = -1;
};