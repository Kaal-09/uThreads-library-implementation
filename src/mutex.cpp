#include "uthread/uthread.h"
#include "scheduler.h"
#include <queue>
#include "mutex.h"

static Schedular schedular;

void Mutex::lock() {
    schedular.disable_preemption();
    if(!locked) {
        locked = true;
        schedular.enable_preemption();
        return;
    }

    TCB* current_tcb = schedular.get_current_tcb();
    wait_queue.push(current_tcb);

    schedular.enable_preemption();
    schedular.block_current();
}

void Mutex::unlock() {
    schedular.disable_preemption();
    if (wait_queue.empty()) {
        locked = false;
    } else {
        TCB* next = wait_queue.front();
        wait_queue.pop();

        schedular.unblock(next);
        if(wait_queue.empty()) locked = false;
    }
    schedular.enable_preemption();
}