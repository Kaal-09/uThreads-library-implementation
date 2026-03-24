#include "scheduler.h"
#include <ucontext.h>


static Schedular schedular;
static size_t thread_count = 0;
bool preemption_disabled = false;

void Schedular::init(){
    current = 0;
}

void Schedular::add_thread(TCB* tcb){
    threads.push_back(tcb); 
}

void Schedular::yield(){
    if (preemption_disabled)
        return;

    if(threads.size() <= 1)
        return;

    int prev = current;
    if (threads[prev]->state == ThreadState::RUNNING) {
        threads[prev]->state = ThreadState::READY;
    }

    do {
        current = (current + 1) % threads.size();
    } while (threads[current]->state == ThreadState::FINISHED || threads[current]->state == ThreadState::BLOCKED);
    threads[current]->state = ThreadState::RUNNING;

    swapcontext(
        &threads[prev]->context,
        &threads[current]->context
    );
}

void Schedular::block_current() {
    threads[current]->state = ThreadState::BLOCKED;
    yield();
}

void Schedular::unblock(TCB* tcb) {
    if (tcb->state == ThreadState::BLOCKED) {
        tcb->state = ThreadState::READY;
    }
}

void Schedular::exit_current() {
    threads[current]->state = ThreadState::FINISHED;
    yield();
}

TCB* Schedular::get_current_tcb() {
    return threads[current];
} 

void Schedular::disable_preemption() {
    preemption_disabled = true;
}

void Schedular::enable_preemption() {
    preemption_disabled = false;
}