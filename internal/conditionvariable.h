#pragma once

#include <queue>
#include "scheduler.h"
#include "mutex.h"

class ConditionVariable {
private:
    std::queue<TCB*> waiters;

public:
    void wait(Mutex& m);
    void signal();
    void broadcast();
};