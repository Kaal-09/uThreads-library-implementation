#include "conditionvariable.h"
#include "scheduler.h"

static ConditionVariable conditionvariable;
static Schedular schedular;

void ConditionVariable::wait(Mutex& m) {
    schedular.disable_preemption();

    TCB* current = schedular.get_current_tcb();
    waiters.push(current);

    m.unlock();

    schedular.enable_preemption();
    schedular.block_current();

    m.lock();
}

void ConditionVariable::signal() {
    schedular.disable_preemption();

    if (!waiters.empty()) {
        TCB* t = waiters.front();
        waiters.pop();
        schedular.unblock(t);
    }

    schedular.enable_preemption();
}

void ConditionVariable::broadcast() {
    schedular.disable_preemption();

    while (!waiters.empty()) {
        TCB* t = waiters.front();
        waiters.pop();
        schedular.unblock(t);
    }

    schedular.enable_preemption();
}