#include "scheduler.h"
#include <csetjmp>

static Schedular schedular;

void Schedular::init(){
    current = 0;
}

void Schedular::add_thread(TCB* tcb){
    threads.push_back(tcb);
}

void Schedular::yield(){
    int prev = current;
    current = (current+1)%threads.size();

    if(setjmp(threads[prev]->context) == 0){
        longjmp(threads[current]->context, 1);
    }
}

void Schedular::exit_current() {
    threads[current]->state = ThreadState::FINISHED;
    yield();
}