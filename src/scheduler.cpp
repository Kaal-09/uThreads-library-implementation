#include "scheduler.h"
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>

static Schedular schedular;
static size_t thread_count = 0;
volatile sig_atomic_t reschedule_requested = 0;
bool preemption_disabled = false;

static void timer_handler(int signum) {
    write(1, "T\n", 2);
    if (!preemption_disabled) {
        reschedule_requested = 1;
    }
}

void Schedular::init(){
    current = 0;

    struct sigaction sa;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;

    sigaction(SIGALRM, &sa, nullptr);

    struct itimerval timer;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100000;
    timer.it_value = timer.it_interval;

    setitimer(ITIMER_REAL, &timer, nullptr);
}

void Schedular::add_thread(TCB* tcb){
    threads.push_back(tcb); 
}

void Schedular::yield(){
    if (preemption_disabled)
        return;

    reschedule_requested = 0;
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

bool Schedular::should_preempt() {
    return reschedule_requested;
}

void Schedular::clear_preempt_flag() {
    reschedule_requested = 0;
}