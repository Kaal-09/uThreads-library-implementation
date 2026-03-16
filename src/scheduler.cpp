#include "scheduler.h"
#include <ucontext.h>

static Schedular schedular;
static size_t thread_count = 0;

void Schedular::init(){
    current = 0;
}

void Schedular::add_thread(TCB* tcb){
    threads.push_back(tcb); 
}

void Schedular::yield(){
    if(threads.size() <= 1)
        return;

    int prev = current;

    do {
        current = (current + 1) % threads.size();
    } while (threads[current]->state == ThreadState::FINISHED);

    swapcontext(
        &threads[prev]->context,
        &threads[current]->context
    );
}

void Schedular::exit_current() {
    threads[current]->state = ThreadState::FINISHED;
    yield();
}