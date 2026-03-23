#include "uthread/uthread.h"
#include "scheduler.h"
#include <queue>

static Schedular schedular;

class Mutex {
    private:
        bool locked = false;
        std::queue<TCB*> wait_queue;

    public:
        void lock();
        void unlock();
};

void Mutex::lock() {
    if(!locked) {
        locked = true;
        return;
    }

    TCB* current_tcb = schedular.get_current_tcb();
    wait_queue.push(current_tcb);

    schedular.block_current();
}

void Mutex::unlock() {
    if (wait_queue.empty()) {
        locked = false;
    } else {
        TCB* next = wait_queue.front();
        wait_queue.pop();

        schedular.unblock(next);
    }
}