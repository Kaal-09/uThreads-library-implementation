#pragma once

#include <vector>
#include "tcb.h"

class Mutex {
    public:
        void lock();
        void unlock();

    private:
        bool locked = false;
        std::queue<TCB*> wait_queue;
};